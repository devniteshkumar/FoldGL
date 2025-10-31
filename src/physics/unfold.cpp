#include "physics/unfold.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

using namespace pdb;

static inline btVector3 toBt(const glm::vec3& v){ return btVector3(v.x, v.y, v.z); }
static inline glm::vec3 toGlm(const btVector3& v){ return glm::vec3(v.x(), v.y(), v.z()); }

// Build orthonormal basis with X aligned to axis
void UnfoldSim::buildOrthonormalBasis(const btVector3& axis, btMatrix3x3& basisOut){
    btVector3 x = axis.normalized();
    btVector3 a = (btFabs(x.x()) < 0.9f) ? btVector3(1,0,0) : btVector3(0,1,0);
    btVector3 z = x.cross(a).normalized();
    btVector3 y = z.cross(x).normalized();
    basisOut.setValue(x.x(), y.x(), z.x(),
                      x.y(), y.y(), z.y(),
                      x.z(), y.z(), z.z());
}

btTransform UnfoldSim::makeFrame(const btVector3& localOrigin, const btVector3& axis){
    btMatrix3x3 basis;
    buildOrthonormalBasis(axis, basis);
    btTransform t; t.setIdentity();
    t.setOrigin(localOrigin);
    t.setBasis(basis);
    return t;
}

UnfoldSim::UnfoldSim(const Model& model){
    // World
    collisionConfig_ = std::make_unique<btDefaultCollisionConfiguration>();
    dispatcher_ = std::make_unique<btCollisionDispatcher>(collisionConfig_.get());
    broadphase_ = std::make_unique<btDbvtBroadphase>();
    solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();
    world_ = std::make_unique<btDiscreteDynamicsWorld>(dispatcher_.get(), broadphase_.get(), solver_.get(), collisionConfig_.get());
    world_->setGravity(btVector3(0,0,0));

    // Shape
    sphereShape_ = std::make_unique<btSphereShape>(0.5f);

    // Collect CA atoms in chain order
    std::vector<glm::vec3> caPos;
    std::vector<std::pair<int,int>> chainResidueIndices; // (chain index, residue index) positions for mapping if needed
    for (const auto& chain : model.chains){
        for (const auto& res : chain->residues){
            for (const auto& atom : res->atoms){
                if (atom->name == "CA"){
                    caPos.emplace_back((float)atom->x, (float)atom->y, (float)atom->z);
                }
            }
        }
    }

    bodies_.reserve(caPos.size());

    // Create rigid bodies for each CA
    btScalar mass = 1.0f;
    btVector3 inertia(0,0,0);
    sphereShape_->calculateLocalInertia(mass, inertia);

    for (const auto& p : caPos){
        btTransform tr; tr.setIdentity(); tr.setOrigin(toBt(p));
        auto* motion = new btDefaultMotionState(tr);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motion, sphereShape_.get(), inertia);
        rbInfo.m_friction = 0.5f;
        rbInfo.m_linearDamping = 0.05f;
        rbInfo.m_angularDamping = 0.05f;
        auto* body = new btRigidBody(rbInfo);
        body->setActivationState(DISABLE_DEACTIVATION);
        world_->addRigidBody(body);
        bodies_.push_back(body);
    }

    // Create constraints between consecutive CA atoms within each chain
    size_t n = bodies_.size();
    for (size_t i = 0; i + 1 < n; ++i){
        btRigidBody* A = bodies_[i];
        btRigidBody* B = bodies_[i+1];
        btVector3 pa = A->getWorldTransform().getOrigin();
        btVector3 pb = B->getWorldTransform().getOrigin();
        btVector3 d = pb - pa;
        btScalar len = d.length();
        if (len < btScalar(1e-4)) continue;
        btVector3 axis = d / len; // desired bond axis
        btVector3 mid = (pa + pb) * btScalar(0.5);

        // Local frames: origins at bond midpoint relative to each body; X aligned with axis
        btVector3 localA = mid - pa; // equals +0.5*d in world since no rotation yet
        btVector3 localB = mid - pb; // equals -0.5*d
        btTransform frameA = makeFrame(localA, axis);
        btTransform frameB = makeFrame(localB, axis);

        auto* dof = new btGeneric6DofConstraint(*A, *B, frameA, frameB, true);
        // Lock all linear DOF (frames coincide at midpoint)
        dof->setLinearLowerLimit(btVector3(0,0,0));
        dof->setLinearUpperLimit(btVector3(0,0,0));
        // Allow twist around X only; lock swing (Y,Z)
        dof->setAngularLowerLimit(btVector3(-SIMD_PI, 0, 0));
        dof->setAngularUpperLimit(btVector3( SIMD_PI, 0, 0));

        // Make constraint stiff
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 0);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 0);
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 1);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 1);
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 2);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 2);
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 3);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 3);
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 4);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 4);
        dof->setParam(BT_CONSTRAINT_STOP_ERP, 0.9f, 5);
        dof->setParam(BT_CONSTRAINT_STOP_CFM, 0.0f, 5);

        world_->addConstraint(dof, true);
        constraints_.push_back(dof);
    }
}

UnfoldSim::~UnfoldSim(){
    for (auto* c : constraints_) { world_->removeConstraint(c); delete c; }
    for (auto* b : bodies_) { world_->removeRigidBody(b); delete b->getMotionState(); delete b; }
}

void UnfoldSim::step(float dt){
    world_->stepSimulation(btScalar(dt), 8, btScalar(std::max(1e-3f, dt/4.0f)));
}

void UnfoldSim::applyPulling(float magnitude){
    if (bodies_.size() < 2) return;
    btRigidBody* a = bodies_.front();
    btRigidBody* b = bodies_.back();
    btVector3 pa = a->getWorldTransform().getOrigin();
    btVector3 pb = b->getWorldTransform().getOrigin();
    btVector3 dir = (pb - pa);
    btScalar len = dir.length();
    if (len < btScalar(1e-6)) return;
    dir /= len;
    btVector3 f = dir * magnitude;
    a->applyCentralForce(-f);
    b->applyCentralForce( f);
}

std::vector<glm::vec3> UnfoldSim::getCAPositions() const{
    std::vector<glm::vec3> out; out.reserve(bodies_.size());
    for (auto* b : bodies_){
        out.emplace_back(toGlm(b->getWorldTransform().getOrigin()));
    }
    return out;
}

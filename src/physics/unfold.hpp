#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>
#include "pdb/model.hpp"

class UnfoldSim {
public:
    // Build a CA-trace physics model from a PDB model
    explicit UnfoldSim(const pdb::Model& model);
    ~UnfoldSim();

    // Step simulation
    void step(float dt);

    // Apply a pulling force on the terminal CA atoms to drive unfolding
    void applyPulling(float magnitude);

    // Current CA positions in the same order used to build the sim
    std::vector<glm::vec3> getCAPositions() const;

    // Count of CA nodes
    size_t size() const { return bodies_.size(); }

private:
    // Bullet world
    std::unique_ptr<btDefaultCollisionConfiguration> collisionConfig_;
    std::unique_ptr<btCollisionDispatcher> dispatcher_;
    std::unique_ptr<btDbvtBroadphase> broadphase_;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
    std::unique_ptr<btDiscreteDynamicsWorld> world_;

    // Shared shapes
    std::unique_ptr<btCollisionShape> sphereShape_;

    // Bodies representing CA atoms (in chain order)
    std::vector<btRigidBody*> bodies_;
    std::vector<btTypedConstraint*> constraints_;

    // Helpers
    static btTransform makeFrame(const btVector3& localOrigin, const btVector3& axis);
    static void buildOrthonormalBasis(const btVector3& axis, btMatrix3x3& basisOut);
};

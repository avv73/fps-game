#include "ZombieNode.h"

ZombieNode::ZombieNode(Zombie* z)
	: ModelNode("zombie", "./models/zombie/zombie.obj")
{
	zombie = z;
}

void ZombieNode::Visualize(const glm::mat4& transform)
{
	if (zombie->GetHealth() <= 0)
		return;

	ModelNode::Visualize(transform);
}

void ZombieNode::TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits, bool isShot)
{
	ModelNode::TraverseIntersection(orig, dir, hits, isShot);

	if (hits.size() > 0 && isShot)
	{
		if (hits[hits.size() - 1]->intersectedNode == this)
		{
			zombie->DecreaseHealth();
		}
	}

}
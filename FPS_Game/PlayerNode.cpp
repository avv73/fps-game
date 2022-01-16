#include "PlayerNode.h"
#include "Player.h"

PlayerNode::PlayerNode(Player* pl)
	: ModelNode("playerNode")
{
	this->pl = pl;
	sphere = new BoundingSphere(this, pl->camera->pos, 0.5f);
}

void PlayerNode::UpdateBoundingPosition(const glm::vec3& pos)
{
	sphere->SetWorldCenter(pos);
}

void PlayerNode::Visualize(const glm::mat4& transform) { }

void PlayerNode::TraverseIntersection(const glm::vec3& orig, const glm::vec3& dir, std::vector<Intersection*>& hits, bool isShot)
{
	ModelNode::TraverseIntersection(orig, dir, hits, isShot);

	if (hits.size() > 0 && isShot)
	{
		if (hits[hits.size() - 1]->intersectedNode == this) // ???????????????
		{
			this->pl->DecreaseHealth();
		}
	}
}

#include "BranchTask.h"



BranchTask::BranchTask():Task()
{
}

BranchTask::BranchTask(BehaviorTree * tree, Task * control): Task(tree, control)
{
}


BranchTask::~BranchTask()
{
}

void BranchTask::run()
{
	//called from child class to continue running a task that is running.
	/*if (status == RUNNING)
	{*/
		int i = 0;
		while (i < children.size() && children[i]->getStatus() != RUNNING)
		{
			i++;
		}
		children[i]->run();
	//}
}

void BranchTask::resetTask()
{
	for (auto task : children)
	{
		task->resetTask();
	}
}

bool BranchTask::addChildToTask(Task * childTask)
{
	if (childTask != nullptr)
	{
		children.push_back(childTask);
		return true;
	}
	else return false;
}

int BranchTask::getChildCount()
{
	return children.size();
}

Task * BranchTask::getChild(int index)
{
	if (index <= getChildCount())
	{
		return (children[index]);
	}
	else
	{
		return nullptr;
	}
}

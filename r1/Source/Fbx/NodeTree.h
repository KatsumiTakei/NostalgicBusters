/**
 *	@file	NodeTree.h
 *	@date	2017 / 9 / 27
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <stdlib.h>


/**
 *  @class	NodeTree
 *  @brief	木構造
 */
template<class T>
class NodeTree
{
private:
	T *root_;
	T *end_;

	int count_;

public:

	NodeTree::NodeTree()
		: root_(nullptr), end_(nullptr), count_(0)
	{
	}

	virtual NodeTree::~NodeTree()
	{
		for (T *type = root_; type != nullptr; )
		{
			T *temp = type;
			type = type->next_;
			delete temp;
		}
		count_ = 0;
	}

public:

	T* GetRoot() const{ return root_; }
	int Size() const { return count_ + 1; }

public:
	/**
	 *	@fn				AddChild
	 *	@brief			親子の構築
	 *	@param[in,out]	argParent	!<	親ノード
	 *	@param[in,out]	argChild	!<	子ノード
	 */
	void AddChild(T *argParent, T *argChild)
	{
		if (argParent)
		{
			if (argParent->firstChild_)
			{
				argParent->endChild_->next_ = argChild;
				argChild->previous_ = argParent->endChild_;
				argChild->next_ = nullptr;
				argParent->endChild_ = argParent->endChild_->next_;
			}
			else
			{
				argParent->endChild_ = argChild;
				argParent->firstChild_ = argChild;
			}

			argChild->parent_ = argParent;
		}
		else
		{
			if (root_)
			{
				end_->next_ = argChild;
				argChild->next_ = nullptr;
				argChild->previous_ = end_;
				end_ = end_->next_;

			}
			else
			{
				root_ = argChild;
				end_ = argChild;
			}
		}
		argChild->id_ = count_;
		++count_;
	}
	/**
	 *	@fn				AddChild
	 *	@brief			親子の構築
	 *	@param[in,out]	argItem	!<	親ノード
	 *	@retval			Node	!<	最初に見つかったノード
	 *	@retval			nullprt	!<	ノードが存在しない
	 */
	T* GetNextChildFirst(T *argItem)
	{
		if (argItem->firstChild_)
			return argItem->firstChild_;

		if (argItem->next_)
			return argItem->next_;

		for (T *parent = argItem->parent_; parent != nullptr; )
		{
			if (parent->next_)
				return parent->next_;

			parent = parent->parent_;
		}

		return nullptr;
	}

};
/**
 *  @struct	NodeTreeItem
 *  @brief	木構造登録構造体
 */
template<class T>
struct NodeTreeItem
{
public:
	NodeTreeItem::NodeTreeItem()
		: next_(nullptr), previous_(nullptr), parent_(nullptr), firstChild_(nullptr), endChild_(nullptr)
	{
	}

	virtual NodeTreeItem::~NodeTreeItem()
	{
		for (T *item = firstChild_; item != nullptr; )
		{
			T *temp = item;
			item = item->next_;
			delete temp;
		}
	};
public:

	T *next_;
	T *previous_;
	T *parent_;
	T *firstChild_;
	T *endChild_;
	int id_;
};

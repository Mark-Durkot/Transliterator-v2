#ifndef SYLLABLETREE_H
#define SYLLABLETREE_H

#include "../LanguageStructures/SyllablePair.h"

#include <QObject>
#include <QList>
#include <QDebug>

class SyllableTreeNode : public QObject
{
    Q_OBJECT
public:
    SyllableTreeNode(SyllablePair s={}, QObject *parent=nullptr)
        : QObject(parent), syllable(s) {}

    ~SyllableTreeNode() {}

    const SyllablePair &getSyllable() const { return syllable; }

    SyllableTreeNode *childAt(int index)    { return children[index]; }

    const QList<SyllableTreeNode*> &getChildren() const { return children; }

    SyllableTreeNode *addChild(SyllablePair p)
    {
        SyllableTreeNode *newNode = new SyllableTreeNode(p, this);
        children.append(newNode);
        return newNode;
    }

    QList<QList<SyllablePair>> collectChildrenData()
    {
        if (children.isEmpty())
        {
            return QList<QList<SyllablePair>>({{syllable}});
        }

        QList<QList<SyllablePair>> data;

        for (auto *child : children)
        {
            data.append(child->collectChildrenData());
        }
        for (auto &node : data)
        {
            node.push_front(syllable);
        }

        return data;
    }

private:
    SyllablePair syllable;
    QList<SyllableTreeNode*> children;
};

//============================================================================
class SyllableTree : public QObject
{
    Q_OBJECT
public:

    SyllableTree(QObject *parent=nullptr)
        : QObject(parent), rootNode(new SyllableTreeNode({}, this))
    {
    }

    ~SyllableTree() {}

    const SyllableTreeNode *getRootNode() const { return rootNode; }
    SyllableTreeNode *getRootNode()             { return rootNode; }

    QList<QList<SyllablePair>> makeList()
    {
        QList<QList<SyllablePair>> list;

        auto rootNodeChildren = rootNode->getChildren();

        for (auto *node : rootNodeChildren)
        {
            list.append(node->collectChildrenData());
        }

        return list;
    }

private:
    SyllableTreeNode *rootNode;
};

#endif // SYLLABLETREE_H

// Copyright © 2024 - Present, Shlomi Nissan.
// All rights reserved.

#include <gtest/gtest.h>
#include <test_helpers.hpp>

#include <engine/nodes/camera_perspective.hpp>
#include <engine/nodes/mesh.hpp>
#include <engine/nodes/node.hpp>

#pragma region Node Operations

TEST(Node, AddChild) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Add(child);

    EXPECT_EQ(parent->Children().size(), 1);
    EXPECT_EQ(parent->Children()[0], child);
}

TEST(Node, RemoveChild) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Add(child);
    parent->Remove(child);

    EXPECT_TRUE(parent->Children().empty());
}

TEST(Node, RemoveAllChildren) {
    auto parent = engine::Node::Create();
    auto child1 = engine::Node::Create();
    auto child2 = engine::Node::Create();

    parent->Add(child1);
    parent->Add(child2);
    parent->RemoveAllChildren();

    EXPECT_TRUE(parent->Children().empty());
}

#pragma endregion

#pragma region Hierarchy Queries

TEST(Node, IsChild) {
    auto node_1 = engine::Node::Create();
    auto node_2 = engine::Node::Create();
    auto node_3 = engine::Node::Create();

    node_1->Add(node_2);
    node_2->Add(node_3);

    EXPECT_TRUE(node_1->IsChild(node_2.get()));
    EXPECT_TRUE(node_1->IsChild(node_3.get()));
    EXPECT_TRUE(node_2->IsChild(node_3.get()));
    EXPECT_FALSE(node_2->IsChild(node_1.get()));
    EXPECT_FALSE(node_3->IsChild(node_1.get()));
}

TEST(Node, IsChildAfterRemoval) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Add(child);
    EXPECT_TRUE(parent->IsChild(child.get()));

    parent->Remove(child);
    EXPECT_FALSE(parent->IsChild(child.get()));
}

TEST(Node, IsChildSelf) {
    auto node = engine::Node::Create();
    EXPECT_FALSE(node->IsChild(node.get()));
}

TEST(Node, IsChildWithNullptr) {
    auto node = engine::Node::Create();
    EXPECT_FALSE(node->IsChild(nullptr));
}

#pragma endregion

#pragma region Update Transforms

TEST(Node, UpdateTransformsWithoutParent) {
    auto node = std::make_shared<engine::Node>();
    node->SetScale(2.0f);

    node->UpdateTransformHierarchy();

    EXPECT_MAT4_EQ(node->GetWorldTransform(), {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
}

TEST(Node, UpdateTransformsWithParent) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->SetScale(2.0f);
    parent->Add(child);

    parent->UpdateTransformHierarchy();

    EXPECT_MAT4_EQ(child->GetWorldTransform(), {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
}

TEST(Node, DisableTransformAutoUpdate) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();
    child->transform_auto_update = false;

    parent->SetScale(2.0f);
    parent->Add(child);
    parent->UpdateTransformHierarchy();

    EXPECT_MAT4_EQ(parent->GetWorldTransform(), {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });

    EXPECT_MAT4_EQ(child->GetWorldTransform(), {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
}

TEST(Node, MarkTransformedNodeAsUntouched) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Add(child);
    parent->UpdateTransformHierarchy();

    EXPECT_FALSE(child->ShouldUpdateWorldTransform());
}

TEST(Node, MarkDetachedNodesAsTouched) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Add(child);
    parent->UpdateTransformHierarchy();

    parent->Remove(child);

    EXPECT_TRUE(child->ShouldUpdateWorldTransform());
}

#pragma endregion

#pragma region ShouldUpdate Checks

TEST(Node, ShouldUpdateTransformWhenDirty) {
    auto node = engine::Node::Create();
    node->SetScale(0.5f);

    EXPECT_TRUE(node->ShouldUpdateWorldTransform());
}

#pragma endregion

#pragma region Edge Cases

TEST(Node, AddChildWithExistingParent) {
    auto parent1 = engine::Node::Create();
    auto parent2 = engine::Node::Create();
    auto child = engine::Node::Create();

    parent1->Add(child);
    parent2->Add(child);

    EXPECT_EQ(parent1->Children().size(), 0);
    EXPECT_EQ(parent2->Children().size(), 1);
    EXPECT_EQ(child->Parent(), parent2.get());
}

TEST(Node, RemoveNonexistentChild) {
    auto parent = engine::Node::Create();
    auto child = engine::Node::Create();

    parent->Remove(child);

    EXPECT_TRUE(parent->Children().empty());
    EXPECT_EQ(child->Parent(), nullptr);
}

#pragma endregion

#pragma region Type Helpers

TEST(Node, TypeCheck) {
    auto temp = engine::CameraPerspective::Create();
    engine::Node* node = temp.get();

    EXPECT_TRUE(node->Is<engine::CameraPerspective>());
    EXPECT_FALSE(node->Is<engine::Mesh>());
}

TEST(Node, TypeCast) {
    auto temp = engine::CameraPerspective::Create();
    engine::Node* node = temp.get();

    auto camera = node->As<engine::CameraPerspective>();
    EXPECT_NE(camera, nullptr);

    auto mesh = node->As<engine::Mesh>();
    EXPECT_EQ(mesh, nullptr);
}

#pragma endregion
#include <gtest/gtest.h>
#include "jsonmodel.h"

TEST(JsonModelTest, CreateTreeWithSimpleObject) {
    JsonModel model;
    QJsonObject simpleObject{
        {"name", "John Doe"},
        {"age", 30}
    };
    QModelIndex parentIndex;

    model.createTree(simpleObject, simpleObject, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "object {2}"));
    EXPECT_EQ(model.rowCount(model.rootIndex()), 2);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "name: John Doe"));
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "age: 30"));

    model.clear();
    EXPECT_FALSE(model.hasElement(model.rootIndex(), "object {2}"));
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithEmptyObject) {
    JsonModel model;
    QJsonObject emptyObject;
    QModelIndex parentIndex;

    model.createTree(emptyObject, emptyObject, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "object {0}"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithEmptyArray) {
    JsonModel model;
    QJsonArray emptyArray;
    QModelIndex parentIndex;

    model.createTree(emptyArray, emptyArray, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "array [0]"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithSimpleArray) {
    JsonModel model;
    QJsonArray simpleArray{
        "apple",
        "banana"
    };
    QModelIndex parentIndex;

    model.createTree(simpleArray, simpleArray, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "array [2]"));
    EXPECT_EQ(model.rowCount(model.rootIndex()), 2);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "0: apple"));
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "1: banana"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithArrayWithNestedObject) {
    JsonModel model;
    QJsonArray arrayWithObject{
        "apple",
        QJsonObject{
            {"name", "Orange"},
            {"color", "Orange"}
        },
        "cherry"
    };
    QModelIndex parentIndex;

    model.createTree(arrayWithObject, arrayWithObject, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "array [3]"));

    QModelIndex childIndex = model.index(0, 0);
    EXPECT_EQ(model.rowCount(childIndex), 3);
    EXPECT_TRUE(model.hasElement(childIndex, "0: apple"));
    QModelIndex objectIndex = model.index(1, 0, childIndex);
    EXPECT_TRUE(model.hasElement(childIndex, "2: cherry"));
    EXPECT_EQ(model.rowCount(objectIndex), 2);
    EXPECT_TRUE(model.hasElement(objectIndex, "name: Orange"));
    EXPECT_TRUE(model.hasElement(objectIndex, "color: Orange"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithArrayWithNestedArray) {
    JsonModel model;
    QJsonArray arrayWithArray{
        "apple",
        QJsonArray{
            "grape",
            "kiwi"        },
        "cherry"
    };
    QModelIndex parentIndex;

    model.createTree(arrayWithArray, arrayWithArray, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "array [3]"));

    QModelIndex childIndex = model.index(0, 0);
    EXPECT_EQ(model.rowCount(childIndex), 3);
    EXPECT_TRUE(model.hasElement(childIndex, "0: apple"));
    EXPECT_TRUE(model.hasElement(childIndex, "1 [2]"));
    EXPECT_TRUE(model.hasElement(childIndex, "2: cherry"));
    QModelIndex nestedArrayIndex = model.index(1, 0, childIndex);
    EXPECT_EQ(model.rowCount(nestedArrayIndex), 2);
    EXPECT_TRUE(model.hasElement(nestedArrayIndex, "0: grape"));
    EXPECT_TRUE(model.hasElement(nestedArrayIndex, "1: kiwi"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithPrimitiveValues) {
    JsonModel model;
    QJsonObject primitiveValues{
        {"name", "John Doe"},
        {"age", 30},
        {"isMarried", true},
        {"height", 1.85}
    };
    QModelIndex parentIndex;

    model.createTree(primitiveValues, primitiveValues, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "object {4}"));

    QModelIndex childIndex = model.index(0, 0);
    EXPECT_EQ(model.rowCount(childIndex), 4);
    EXPECT_TRUE(model.hasElement(childIndex, "name: John Doe"));
    EXPECT_TRUE(model.hasElement(childIndex, "age: 30"));
    EXPECT_TRUE(model.hasElement(childIndex, "isMarried: true"));
    EXPECT_TRUE(model.hasElement(childIndex, "height: 1.85"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}


TEST(JsonModelTest, CreateTreeWithComplexNestedStructure) {
    JsonModel model;
    QJsonObject complexObject{
        {"name", "John Doe"},
        {"age", 30},
        {"address", QJsonObject{
                {"street", "123 Main St"},
                {"city", "Anytown"},
                {"zip", "12345"}
            }},
        {"hobbies", QJsonArray{
                "reading",
                "hiking",
                "coding"
            }},
        {"favoriteColors", QJsonArray{
                "red",
                "green",
                "blue"
            }},
        {"isMarried", true},
        {"height", 1.85},
        {"family", QJsonArray{
                QJsonObject{
                    {"name", "Jane Doe"},
                    {"age", 28}
                },
                QJsonObject{
                    {"name", "Little John"},
                    {"age", 5}
                }
            }}
    };
    QModelIndex parentIndex;

    model.createTree(complexObject, complexObject, parentIndex);

    EXPECT_EQ(model.rowCount(), 1);
    EXPECT_TRUE(model.hasElement(model.rootIndex(), "object {8}"));
    QModelIndex childIndex = model.index(0, 0);
    EXPECT_EQ(model.rowCount(childIndex), 8);
    EXPECT_TRUE(model.hasElement(childIndex, "name: John Doe"));
    EXPECT_TRUE(model.hasElement(childIndex, "age: 30"));
    EXPECT_TRUE(model.hasElement(childIndex, "address {3}"));
    EXPECT_TRUE(model.hasElement(childIndex, "hobbies [3]"));
    EXPECT_TRUE(model.hasElement(childIndex, "favoriteColors [3]"));
    EXPECT_TRUE(model.hasElement(childIndex, "isMarried: true"));
    EXPECT_TRUE(model.hasElement(childIndex, "height: 1.85"));
    EXPECT_TRUE(model.hasElement(childIndex, "family [2]"));

    QModelIndex addressIndex = model.index(0, 0, model.rootIndex());
    EXPECT_EQ(model.rowCount(addressIndex), 3);
    EXPECT_TRUE(model.hasElement(addressIndex, "street: 123 Main St"));
    EXPECT_TRUE(model.hasElement(addressIndex, "city: Anytown"));
    EXPECT_TRUE(model.hasElement(addressIndex, "zip: 12345"));

    QModelIndex hobbiesIndex = model.index(5, 0, model.rootIndex());
    EXPECT_EQ(model.rowCount(hobbiesIndex), 3);
    EXPECT_TRUE(model.hasElement(hobbiesIndex, "0: reading"));
    EXPECT_TRUE(model.hasElement(hobbiesIndex, "1: hiking"));
    EXPECT_TRUE(model.hasElement(hobbiesIndex, "2: coding"));

    QModelIndex colorsIndex = model.index(3, 0, model.rootIndex());
    EXPECT_EQ(model.rowCount(colorsIndex), 3);
    EXPECT_TRUE(model.hasElement(colorsIndex, "0: red"));
    EXPECT_TRUE(model.hasElement(colorsIndex, "1: green"));
    EXPECT_TRUE(model.hasElement(colorsIndex, "2: blue"));

    QModelIndex familyIndex = model.index(2, 0, model.rootIndex());
    EXPECT_EQ(model.rowCount(familyIndex), 2);
    EXPECT_TRUE(model.hasElement(familyIndex, "0 {2}"));

    QModelIndex familyMember1Index = model.index(0, 0, familyIndex);
    EXPECT_EQ(model.rowCount(familyMember1Index), 2);
    EXPECT_TRUE(model.hasElement(familyMember1Index, "name: Jane Doe"));
    EXPECT_TRUE(model.hasElement(familyMember1Index, "age: 28"));

    QModelIndex familyMember2Index = model.index(1, 0, familyIndex);
    EXPECT_EQ(model.rowCount(familyMember2Index), 2);
    EXPECT_TRUE(model.hasElement(familyMember2Index, "name: Little John"));
    EXPECT_TRUE(model.hasElement(familyMember2Index, "age: 5"));

    model.clear();
    EXPECT_EQ(model.rowCount(), 0);
}

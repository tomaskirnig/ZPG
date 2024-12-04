#include "Transformation.h"

Transformation::Transformation() {
    addTransformation(new Translation(glm::vec3(0.0f)));
    addTransformation(new Rotation(0.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
    addTransformation(new Rotation(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    addTransformation(new Rotation(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    addTransformation(new Scale(glm::vec3(1.0f)));
}

Transformation::~Transformation() {
    for (auto* child : transformations) {
        delete child;
    }
}

// Returns the transformation matrix
glm::mat4 Transformation::getMatrix() const {
    glm::mat4 result = glm::mat4(1.0f);
    for (const auto& child : transformations) {
        result *= child->getMatrix();
    }
    return result;
}

// Position
void Transformation::moveObject(const glm::vec3& offset) {
    Translation* translation = nullptr;

    // Search for an existing Translation transformation
    for (auto* trans : transformations) {
        translation = dynamic_cast<Translation*>(trans);
        if (translation) {
            break;
        }
    }

    // Modify the existing Translation or add a new one
    if (translation) {
        translation->offset += offset; 
    }
    else {
        translation = new Translation(offset);
        addTransformation(translation);
    }
}

void Transformation::setPosition(const glm::vec3& position) {
    Translation* translation = nullptr;

    for (auto* trans : transformations) {
        translation = dynamic_cast<Translation*>(trans);
        if (translation) {
            break;
        }
    }

    if (translation) {
        translation->offset = position; 
    }
    else {
        translation = new Translation(position);
        addTransformation(translation);
    }
}

// Returns the position
glm::vec3 Transformation::getPosition(){
    for (const auto* trans : transformations) {
        const Translation* translation = dynamic_cast<const Translation*>(trans);
        if (translation) {
            return translation->offset;
        }
    }
    return glm::vec3(0.0f);  // Default position if no Translation found
}

void Transformation::rotateObject(float angleRadians, const glm::vec3& axis) {
    // Check if there's already a Rotation with the same axis
    for (auto* trans : transformations) {
        Rotation* rotation = dynamic_cast<Rotation*>(trans);
        if (rotation && rotation->axis == axis) {
            rotation->angle += angleRadians;
            return;
        }
    }

    // If no matching axis found, create a new Rotation
    Rotation* newRotation = new Rotation(angleRadians, axis);
    addTransformation(newRotation);
}

// Sets new rotation matrix
void Transformation::setRotation(float angleRadians, const glm::vec3& axis) {
    for (auto* trans : transformations) {
        Rotation* rotation = dynamic_cast<Rotation*>(trans);
        if (rotation && rotation->axis == axis) {
            rotation->angle = angleRadians;
            return;
        }
    }

    Rotation* newRotation = new Rotation(angleRadians, axis);
    addTransformation(newRotation);
}

// Scale object by modifying or adding a Scale transformation
void Transformation::scaleObject(const glm::vec3& scaleFactor) {
    for (auto* trans : transformations) {
        if (Scale* scaling = dynamic_cast<Scale*>(trans)) {
            scaling->scaleFactor *= scaleFactor;
            return;
        }
    }
    addTransformation(new Scale(scaleFactor));
}


// Sets new scale
void Transformation::setScale(const glm::vec3& scale) {
    Scale* scaling = nullptr;

    for (auto* trans : transformations) {
        scaling = dynamic_cast<Scale*>(trans);
        if (scaling) {
            break;
        }
    }

    if (scaling) {
        scaling->scaleFactor = scale;
    }
    else {
        scaling = new Scale(scale);
        addTransformation(scaling);
    }
}


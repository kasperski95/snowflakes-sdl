#include "../Objects.h"


Actor::Actor()
    : _transformationMatrix(3,3)
    {}

void Actor::parent(Actor* parentToSet) {
    _parent = parentToSet;
}


void Actor::location(float x, float y) {
    _translate(x, y);
}


Point Actor::location() {
    return Point(_transformationMatrix.get(0,2), _transformationMatrix.get(1,2));
}


math::Matrix<float> Actor::transformationMatrix() {
    math::Matrix<float> tmpTransformationMatrix = _transformationMatrix;
    if (_parent) {
        tmpTransformationMatrix = _transformationMatrix * _parent->transformationMatrix();
    }
    return tmpTransformationMatrix;
}


void Actor::motionBlur(bool value) {
    _motionBlur = value;
}


bool Actor::motionBlur() {
    bool parentMotionBlur = false;
    if (_parent) {
        parentMotionBlur = _parent->motionBlur();
    }
    return parentMotionBlur || _motionBlur;
}


void Actor::_translate(math::Matrix<float> m) {
    this->_translate(m.get(0,0), m.get(1,0));
}


void Actor::_translate(float x, float y) {
    _transformationMatrix.set(0, 2, x);
    _transformationMatrix.set(1, 2, y);
}

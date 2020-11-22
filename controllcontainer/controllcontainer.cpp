#include "controllcontainer.h"

CControllContainer::CControllContainer( CControllContainer* parent) {
    if (parent == nullptr)
        _Parent = this;
    else
        _Parent = parent;

    _Child = nullptr;
}

void CControllContainer::addContainer(CControllContainer * parent) {
    CControllContainer * child = new CControllContainer(parent);
    this->_Child = child;
}

bool CControllContainer::removeContainer(CControllContainer *container) {
    // do somthing later
}

void CControllContainer::enableChilds() {

}

void CControllContainer::disableChilds() {

}

void CControllContainer::rename(std::string theNewName){
    _Name = theNewName;
}

bool CControllContainer::addControll2D(CControllContainer *container, Base2D *controll) {
    return true;
}

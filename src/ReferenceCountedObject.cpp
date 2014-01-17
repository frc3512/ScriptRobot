#include "ReferenceCountedObject.h"

ReferenceCountedObject::ReferenceCountedObject()
{
    m_references = 0;

}

void ReferenceCountedObject::addRef()
{
    m_references++;

}

void ReferenceCountedObject::remRef()
{
    m_references == 0 ? m_references = 0 : m_references--;
    if(m_references == 0)
    {
        delete this;

    }

}

unsigned int ReferenceCountedObject::references()
{
    return m_references;

}

ReferenceCountedObject::~ReferenceCountedObject()
{

}

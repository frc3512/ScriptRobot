#ifndef REFERENCECOUNTEDOBJECT_H_
#define REFERENCECOUNTEDOBJECT_H_

#if defined _WIN32 || __linux__
#include <stdint.h>
#elif defined __VXWORKS__
#include <types/vxTypes.h>
#endif

class ReferenceCountedObject
{
public:
    ReferenceCountedObject();

    void addRef();

    void remRef();

    uint32_t references();

protected:
    virtual ~ReferenceCountedObject();

    uint32_t m_references;

};

#endif /* REFRENCECOUNTEDOBJECT_H_ */

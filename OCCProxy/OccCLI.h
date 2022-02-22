#pragma once

#if ! defined(_MSC_VER) || ! defined(_MANAGED)
#error This file is usable only in C++/CLI (.NET) programs
#endif

#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

//! Template CLI class providing the way to encapsulate instance of C++ 
//! class as a field in the C++/CLI (ref) class. 
//!
//! It can be helpful to encapsulate OCCT Handles, maps, arrays, etc.
//!
//! Use of variable of the Haft type is very similar to that of encapsulated 
//! class:
//! - Default constructor creates default-constructed C++ instance
//! - Non-default construction is possible by copy or by initialization from
//!   compatible pointer (e.g. Haft for Handle can be initialized by pointer 
//!   returned by operator new for a handled class)
//! - Underlying C++ instance is accessed by operator ()

namespace OCCT
{
    template <class CPPClass>
    public ref class OccCLI
    {
    protected:
        enum class InitMode
        {
            Uninitialized
        };

    public:
        //! Initialize CLI Haft object by default-constructed C++ object
        OccCLI(InitMode init) {}

        OccCLI()
        {
            myPtr = new CPPClass;
        }

        //! Initialize CLI Haft object by compatible C++ pointer
        template <class T>
        OccCLI(const T* aPtr)
        {
            myPtr = new CPPClass(aPtr);
        }

        //! Initialize CLI Haft object by C++ class object
        OccCLI(const CPPClass& aPtr)
        {
            myPtr = new CPPClass(aPtr);
        }

        //! Destructor - invoked explicitly by delete, or automatically 
        //! when local variable is scoped out
        ~OccCLI()
        {
            this->Nullify();
        }

        //! Finalizer - called undeterministically by garbage collector
        !OccCLI()
        {
            this->Nullify();
        }

        //! Function call operator is provided to access underlying C++ object
        CPPClass& operator () () { return *myPtr; }

    protected:
        //! Invalidate the haft
        void Nullify()
        {
            delete myPtr;
            myPtr = 0;
        }

    protected:
        CPPClass* myPtr;
    };
}

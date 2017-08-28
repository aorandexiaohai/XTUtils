#pragma once
/*
Before you delete a ptr, you should know all its class definition.
If not, undefied behaviour.
*/
template<class T>
inline void SafeDelete(T* ptr) {
	(void)(sizeof T);
	delete ptr;
}

template<class T>
inline void SafeArrayDelete(T* ptr) {
	(void)(sizeof T);
	delete [] ptr;
}

#define SAFEDELETE(ptr)\
{\
	auto tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec = (ptr);\
	(void)(sizeof *tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec);\
	delete tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec;\
}

#define SAFEARRAYDELETE(ptr)\
{\
	auto tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec = (ptr);\
	(void)(sizeof *tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec);\
	delete [] tmp_8fd7be93_30dd_472b_91fe_8bad0d0edfec;\
}
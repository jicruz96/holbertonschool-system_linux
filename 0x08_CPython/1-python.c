#include <Python.h>

/**
 * print_python_list - prints some basic info for Python lists
 * @p: pointer to Python List Object
 **/
void print_python_list(PyObject *p)
{
	Py_ssize_t list_size, i;
	PyListObject *list = (PyListObject *)p;

	list_size = Py_SIZE(p);
	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", list_size);
	printf("[*] Allocated = %ld\n", list->allocated);
	for (i = 0; i < list_size; i++)
		printf("Element %ld: %s\n", i, Py_TYPE(list->ob_item[i])->tp_name);
}

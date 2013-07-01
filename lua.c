#include <Python.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define KEY_MAX 10

int add(int x, int y) {
    return x + y;
}

int det(int x, int y) {
    return x - y;
}


PyObject *test_add(PyObject *self, PyObject *args) {
    int x, y, res;
    if(! PyArg_ParseTuple(args, "ii", &x, &y)) return NULL;
    res = add(x, y);
    return Py_BuildValue("i", res);
}

PyObject *test_det(PyObject *self, PyObject *args) {
    int x, y, res;
    if(! PyArg_ParseTuple(args, "ii", &x, &y)) return NULL;
    res = det(x, y);
    return Py_BuildValue("i", res);
}

char *Get(char *filename, char *key) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, key);
    return (char*)lua_tostring(L, -1);
}

char *Table(char *filename, char *table, char *key) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, table);
    lua_getfield(L, -1, key);
    return (char*)lua_tostring(L, -1);
}

int Run(char *str) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    return luaL_dostring(L, str);
}

PyObject *lua_get(PyObject *self, PyObject *args) {
    char *filename, *key;
    char *res;
    if(!PyArg_ParseTuple(args, "ss", &filename, &key)) return NULL;
    res = Get(filename, key);
    if(!res) res = "nil";
    return Py_BuildValue("s", res);
}

PyObject *lua_table(PyObject *self, PyObject *args) {
    char *filename, *res, *table, *key;
    if(!PyArg_ParseTuple(args, "sss", &filename, &table, &key)) return NULL;
    res = Table(filename, table, key);
    if(!res) res = "nil";
    return Py_BuildValue("s", res);
}

PyObject *lua_run(PyObject *self, PyObject *args) {
    char *str;
    if(!PyArg_ParseTuple(args, "s", &str)) return NULL;
    return Py_BuildValue("i", Run(str));
}

static PyMethodDef lua_methods[] = {
    {"add", test_add, METH_VARARGS, "lua.add(1, 2)"},
    {"det", test_det, METH_VARARGS, "lua.det(3, 2)"},
    {"get", lua_get, METH_VARARGS, "lua.get(filename, key)"},
    {"table", lua_table, METH_VARARGS, "lua.table(filename, table, key)"},
    {"run", lua_run, METH_VARARGS, "lua.run(string)"},
    {NULL, NULL, 0, NULL}
};

void initlua() {
    PyObject *m;
    m = Py_InitModule("lua", lua_methods);
}

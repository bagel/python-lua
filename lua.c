#include <Python.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define KEY_MAX 10

typedef struct {
    char *key;
    char *value;
} item;

char *Get(char *filename, char *key) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, key);
    if(lua_istable(L, -1)) return "istable";
    return (char*)lua_tostring(L, -1);
}

char *Table(char *filename, char *table, char *key) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, table);
    lua_getfield(L, -1, key);
    if(lua_istable(L, -1)) return "istable";
    return (char*)lua_tostring(L, -1);
}

item firstKey(char *filename, char *table) {
    item first;
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, table);
    lua_pushnil(L);
    lua_next(L, -2);
    first.key = (char*)lua_tostring(L, -2);
    if(lua_istable(L, -1))
        first.value = "istable";
    else
        first.value = (char*)lua_tostring(L, -1);
    return first;
}

item nextKey(char *filename, char *table, char *key) {
    item next;
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, table);
    lua_pushstring(L, key);
    lua_next(L, -2);
    next.key = (char*)lua_tostring(L, -2);
    if(lua_istable(L, -1))
        next.value = "istable";
    else
        next.value = (char*)lua_tostring(L, -1);
    return next;
}

char *firstTableKey(char *filename, char *table, char *key) {
    lua_State *L = lua_open();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    lua_getglobal(L, table);
    lua_getfield(L, -1, key);
    lua_pushnil(L);
    lua_next(L, -2);
    if(lua_istable(L, -1)) return "istable";
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

PyObject *lua_firstkey(PyObject *self, PyObject *args) {
    char *filename, *table;
    item res;
    if(!PyArg_ParseTuple(args, "ss", &filename, &table)) return NULL;
    res = firstKey(filename, table);
    //if(!res) res = {"nil", "nil"};
    return Py_BuildValue("{s:s}", res.key, res.value);
}

PyObject *lua_nextkey(PyObject *self, PyObject *args) {
    char *filename, *table, *key;
    item res;
    if(!PyArg_ParseTuple(args, "sss", &filename, &table, &key)) return NULL;
    res = nextKey(filename, table, key);
    //if(!res) res = "nil";
    return Py_BuildValue("{s:s}", res.key, res.value);
}

PyObject *lua_tfirstkey(PyObject *self, PyObject *args) {
    char *filename, *res, *table, *key;
    if(!PyArg_ParseTuple(args, "sss", &filename, &table, &key)) return NULL;
    res = firstTableKey(filename, table, key);
    if(!res) res = "nil";
    return Py_BuildValue("s", res);
}

PyObject *lua_run(PyObject *self, PyObject *args) {
    char *str;
    if(!PyArg_ParseTuple(args, "s", &str)) return NULL;
    return Py_BuildValue("i", Run(str));
}

static PyMethodDef lua_methods[] = {
    {"get", lua_get, METH_VARARGS, "lua.get(filename, key)"},
    {"table", lua_table, METH_VARARGS, "lua.table(filename, table, key)"},
    {"run", lua_run, METH_VARARGS, "lua.run(string)"},
    {"first", lua_firstkey, METH_VARARGS, "lua.first(filename, table)"},
    {"next", lua_nextkey, METH_VARARGS, "lua.next(filename, table, key)"},
    {"tfirst", lua_tfirstkey, METH_VARARGS, "lua.tfirst(filename, table, key)"},
    {NULL, NULL, 0, NULL}
};

void initlua() {
    PyObject *m;
    m = Py_InitModule("lua", lua_methods);
}

/****************************************************************************
** Meta object code from reading C++ file 'AddressBookDB.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../AddressBookDB.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddressBookDB.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AddressBook_t {
    QByteArrayData data[17];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AddressBook_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AddressBook_t qt_meta_stringdata_AddressBook = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AddressBook"
QT_MOC_LITERAL(1, 12, 12), // "showContacts"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 14), // "showContactsDB"
QT_MOC_LITERAL(4, 41, 10), // "addContact"
QT_MOC_LITERAL(5, 52, 12), // "addContactDB"
QT_MOC_LITERAL(6, 65, 11), // "editContact"
QT_MOC_LITERAL(7, 77, 13), // "deleteContact"
QT_MOC_LITERAL(8, 91, 15), // "deleteContactDB"
QT_MOC_LITERAL(9, 107, 19), // "deleteContactFromDB"
QT_MOC_LITERAL(10, 127, 9), // "contactId"
QT_MOC_LITERAL(11, 137, 13), // "searchContact"
QT_MOC_LITERAL(12, 151, 11), // "sortContact"
QT_MOC_LITERAL(13, 163, 12), // "clearContact"
QT_MOC_LITERAL(14, 176, 17), // "showCustomMessage"
QT_MOC_LITERAL(15, 194, 5), // "title"
QT_MOC_LITERAL(16, 200, 7) // "message"

    },
    "AddressBook\0showContacts\0\0showContactsDB\0"
    "addContact\0addContactDB\0editContact\0"
    "deleteContact\0deleteContactDB\0"
    "deleteContactFromDB\0contactId\0"
    "searchContact\0sortContact\0clearContact\0"
    "showCustomMessage\0title\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AddressBook[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      11,    0,   84,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    2,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,

       0        // eod
};

void AddressBook::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AddressBook *_t = static_cast<AddressBook *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showContacts(); break;
        case 1: _t->showContactsDB(); break;
        case 2: _t->addContact(); break;
        case 3: _t->addContactDB(); break;
        case 4: _t->editContact(); break;
        case 5: _t->deleteContact(); break;
        case 6: _t->deleteContactDB(); break;
        case 7: _t->deleteContactFromDB((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->searchContact(); break;
        case 9: _t->sortContact(); break;
        case 10: _t->clearContact(); break;
        case 11: _t->showCustomMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AddressBook::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_AddressBook.data,
    qt_meta_data_AddressBook,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AddressBook::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AddressBook::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AddressBook.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AddressBook::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

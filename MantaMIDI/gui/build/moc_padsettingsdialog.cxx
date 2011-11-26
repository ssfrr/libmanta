/****************************************************************************
** Meta object code from reading C++ file 'padsettingsdialog.h'
**
** Created: Wed Jul 20 23:04:09 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../padsettingsdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'padsettingsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PadSettingsDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      57,   18,   18,   18, 0x08,
      93,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PadSettingsDialog[] = {
    "PadSettingsDialog\0\0"
    "on_padSelectionSpin_valueChanged(int)\0"
    "on_padChannelSpin_valueChanged(int)\0"
    "on_padNoteSpin_valueChanged(int)\0"
};

const QMetaObject PadSettingsDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PadSettingsDialog,
      qt_meta_data_PadSettingsDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PadSettingsDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PadSettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PadSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PadSettingsDialog))
        return static_cast<void*>(const_cast< PadSettingsDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PadSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_padSelectionSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_padChannelSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_padNoteSpin_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

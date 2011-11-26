/****************************************************************************
** Meta object code from reading C++ file 'mantathread.h'
**
** Created: Fri Oct 21 10:03:38 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mantathread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mantathread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MantaThread[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   13,   12,   12, 0x05,
      51,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   12,   12,   12, 0x08,
     103,   12,   12,   12, 0x08,
     129,   12,   12,  124, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MantaThread[] = {
    "MantaThread\0\0output\0MantaConnectedMessage(QString)\0"
    "UpdateStatusMessage(QString)\0"
    "ForceMantaDisconnect()\0RunMantaDiagnostic()\0"
    "void\0RunManta()\0"
};

const QMetaObject MantaThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MantaThread,
      qt_meta_data_MantaThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MantaThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MantaThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MantaThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MantaThread))
        return static_cast<void*>(const_cast< MantaThread*>(this));
    return QThread::qt_metacast(_clname);
}

int MantaThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: MantaConnectedMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: UpdateStatusMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: ForceMantaDisconnect(); break;
        case 3: RunMantaDiagnostic(); break;
        case 4: RunManta(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MantaThread::MantaConnectedMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MantaThread::UpdateStatusMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

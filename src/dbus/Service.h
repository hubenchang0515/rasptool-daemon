#ifndef RASP_DBUS_SERVICE_H
#define RASP_DBUS_SERVICE_H

#include <map>

#include <giomm.h>
#include <glibmm.h>

#include "Object.h"

namespace Rasp
{

namespace DBus
{


class Service
{
public:
    Service(const Glib::ustring& name);
    ~Service() = default;

    /*****************************************************************************
     * @brief 返回服务名
     * @return 服务名
     * ***************************************************************************/
    Glib::ustring name() const noexcept;

    /*****************************************************************************
     * @brief 导出对象
     * @param[in] obj 对象
     * @return 是否成功
     * ***************************************************************************/
    bool exportObject(const Object& obj) noexcept;

    /*****************************************************************************
     * @brief 删除对象
     * @param[in] path 对象路径
     * @return 是否成功
     * ***************************************************************************/
    bool unexportObject(const Glib::ustring& path) noexcept;

    /*****************************************************************************
     * @brief 注册服务
     * @param[in] service 服务
     * @param[in] type 总线类型
     * @return id
     * ***************************************************************************/
    static guint registerService(Service* service, Gio::DBus::BusType type=Gio::DBus::BusType::BUS_TYPE_SESSION);

    /*****************************************************************************
     * @brief 注销服务
     * @param[in] name 名字
     * @param[in] del 是否同时删除对象
     * @return 是否成功 
     * ***************************************************************************/
    static bool unregisterService(const Glib::ustring& name, bool del=false);

    /*****************************************************************************
     * @brief 注销服务
     * @param[in] obj 对象
     * @param[in] del 是否同时删除对象
     * @return 是否成功 
     * ***************************************************************************/
    static bool unregisterService(Service* service, bool del=false);


private:
    Glib::ustring m_name;
    Gio::DBus::InterfaceVTable m_vtable; // DBus 虚表
    guint m_ownerId;

    std::map<Glib::ustring, Object> m_objects;
    std::map<Glib::ustring, guint>  m_objIds;

    /* 服务名 => 服务 */
    static std::map<Glib::ustring, Service*> services;

    /* 对象路径 => 所属服务 */
    static std::map<Glib::ustring, Service*> objServices;

    /*****************************************************************************
     * @brief 回调函数，DBus 获得总线
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onBusAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& name);

    /*****************************************************************************
     * @brief 回调函数，DBus 获得连接名
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onNameAcquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                                const Glib::ustring& name);

    /*****************************************************************************
     * @brief 回调函数，DBus 失去连接名
     * @param[in] connection DBus连接
     * @param[in] name 名字
     * ***************************************************************************/
    static void onNameLost(const Glib::RefPtr<Gio::DBus::Connection>& connection, 
                                const Glib::ustring& name);

    /*****************************************************************************
     * @brief 回调函数，DBus 方法调用
     * @param[in] connection DBus连接
     * @param[in] sender 发送方
     * @param[in] objectPath 对象路径
     * @param[in] interfaceName 接口名
     * @param[in] methodName 方法名
     * @param[in] args 参数
     * @param[in] invocation 
     * ***************************************************************************/
    static void onMethodCall(const Glib::RefPtr<Gio::DBus::Connection>& connection,
                                const Glib::ustring& sender,
                                const Glib::ustring& objectPath,
                                const Glib::ustring& interfaceName,
                                const Glib::ustring& methodName,
                                const Glib::VariantContainerBase& args,
                                const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);
};

}; // namespace DBus

}; // namespace Rasp


#endif // RASP_DBUS_SERVICE_H
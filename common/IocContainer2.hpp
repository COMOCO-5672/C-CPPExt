
// 完整的IOC容器代码
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

namespace std {
class Any {
  public:
    // 默认构造函数
    Any() : m_tpIndex(std::type_index(typeid(void))) {}
    Any(const Any &other) : m_ptr(other.clone()), m_tpIndex(other.m_tpIndex) {}
    Any(Any &&other) : m_ptr(std::move(other.m_ptr)), m_tpIndex(std::move(other.m_tpIndex)) {}

    // 通用的右值构造
    template <class T, class = typename std::enable_if<
                           !std::is_same<std::decay<T>::type, Any>::value, T>::type>
    Any(T &&t)
        : m_ptr(new Derived<std::decay<T>::type>(std::forward<T>(t))),
          m_tpIndex(typeid(std::decay<T>::type))
    {
    }

    // 判断是否为空
    bool isNull() { return !bool(m_ptr); }

    // 是否可以类型转换
    template <class T> bool is() { return m_tpIndex == type_index(typeid(T)); }

    // 类型转换
    template <class T> T &cast()
    {
        if (!is<T>()) {
            cout << "can not cast " << typeid(T).name() << " to " << m_tpIndex.name() << endl;
            throw bad_cast();
        }
        auto ptr = dynamic_cast<Derived<T> *>(m_ptr.get());
        return ptr->m_value;
    }

    Any &operator=(const Any &other)
    {
        if (m_ptr == other.m_ptr) {
            return *this;
        }
        m_ptr = other.clone();
        m_tpIndex = other.m_tpIndex;
        return *this;
    }

  private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;

    // 非模板擦除类型
    struct Base {
        virtual BasePtr clone() const = 0;
    };

    template <typename T> struct Derived : public Base {
        template <typename... Args> Derived(Args &&...args) : m_value(std::forward<Args>(args)...)
        {
        }
        BasePtr clone() const { return BasePtr(new Derived(m_value)); }

        T m_value;
    };

    // 拷贝使用
    BasePtr clone() const
    {
        if (m_ptr) {
            return m_ptr->clone();
        }
        return nullptr;
    }

    BasePtr m_ptr;             // 具体数据
    std::type_index m_tpIndex; // 数据类型
};

} // namespace std

class IocContainer {
  public:
    // 注册一个key对应的类型
    template <class T, class Depend,
              class = std::enable_if<!std::is_same<Depend, void>::value>::type>
    void registerType(std::string key)
    {
        std::function<T *()> func = [] { return new T(new Depend()); };
        registerType(key, func);
    }

    // 简单的对象工厂
    template <class T> void registerType(std::string key)
    {
        std::function<T *()> func = [] { return new T(); };
        registerType(key, func);
    }

    // 根据唯一标志查询对应的构造器 创建对象
    template <class T> T *resolve(std::string key)
    {
        if (m_map.find(key) == m_map.end()) {
            return nullptr;
        }
        Any func = m_map[key];

        // 转换为合适的类型
        std::function<T *()> f = func.cast<std::function<T *()>>();
        return f();
    }

    template <class T> std::shared_ptr<T> resolveShared(std::string key)
    {
        T *t = resolve<T>(key);
        return std::shared_ptr<T>(t);
    }

  private:
    void registerType(std::string key, std::Any func)
    {
        if (m_map.find(key) != m_map.end()) {
            throw std::invalid_argument("this key has exist");
        }
        m_map.emplace(key, func);
    }

  private:
    std::map<std::string, std::Any> m_map;
};

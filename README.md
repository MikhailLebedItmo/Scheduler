# Лабораторная работа 10

Шедулер задач

## Задача

Вашей задачей будет разработать класс отвечающий за выполнение связанных по данным между собой задач.
Часто, чтобы решить какую либо задачу, требуется выполнить граф вычислений, где узел графа это задача, ребро связь между результатом выполнения одной задачи и параметром для запуска другой. Вам предстоит разработать класс **TTaskScheduler** , решающий подобную задачу.

Пример такой задачи - нахождение корней квадратного уравнения (предполагаем что коэффициенты гарантирует что корня 2). Решение подобной задачи в лоб - подразумевает вычисление корня из дискриминанта дважды.  Очевидно, что это не оптимально.


Вот так мог бы выглядеть код для решение данной задачи с помощью TTaskScheduler

```cpp
float a = 1;
float b = -2;
float c = 0;

TTaskScheduler scheduler;

auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));

auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d)}, b, scheduler.getFutureResult<float>(id2));

auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d)}, b, scheduler.getFutureResult<float>(id2));

auto id5 = scheduler.add([]{float a, float v}{return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

auto id6 = scheduler.add([]{float a, float v}{return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

scheduler.executeAll();

std::cout << "x1 = " << scheduler.getResult<int>(id5) << std::endl;
std::cout << "x2 = " << scheduler.getResult<int>(id6) << std::endl;
```

Где getFutureResult это результат выполнения задачи в будущем,

### Публичный интефейс TTaskScheduler

 - **add** - принимает в качестве аргумента задание для него. Возвращает объект описывающий добавленную таску.
 - **getFutureResult<T>** - возвращает объект, из которого в будущем можно получить результат задания, переданного в качестве результата типа Т
 - **getResult<T>** - возвращает результат выполнения задания определенного типа. Вычисляет его если оно еще не подсчитано
 - **executeAll** - выполняет все запланированные задания

### Требования и ограничения к заданиям

  - [Callable object](https://en.cppreference.com/w/cpp/named_req/Callable)
  - Аргументы гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)
  - Возвращаемое значение произвольное, гарантированно [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)
  - Количество аргументов не больше 2

## Ограничения

 Запрещено использовать стандартную библиотеку, за исключением контейнеров и умных указателей.

## Особенности реализации

### FutureResult
Для реализации `FutureResult<T>` было придумано изящное решение, сделать оператор каста к `T&`.
Тогда, когда мы попытаемся вызвать функцию, где в качестве аргумента передаём `FutureResult<T>`, 
он неявно скастится к `T&`, в процессе подсчитав результат(если до этого ещё не был подсчитан).

### Any
Для того чтобы хранить результат выполнения таски, была написана собственная реализация any.
Внутри класс по сути моделирует поведение виртуальных функций, он хранит в качестве поля `man` 
указатель на шаблонную функцию.

```c++
template<typename ValueType>
static void* value_manager(operation op, Any& left, const Any* right) {
    switch (op) {
        case Destroy:
            delete static_cast<ValueType*>(left.value_ptr);
            break;
        case Move:
            left.value_ptr = right->value_ptr;
            left.man = right->man;
            const_cast<Any*>(right)->value_ptr = nullptr;
            const_cast<Any*>(right)->man = nullptr;
            break;
        case Copy:
            left.value_ptr = new ValueType(*static_cast<const ValueType*>(right->value_ptr));
            left.man = right->man;
            break;
    }
    return nullptr;
}
```
И когда ему нужно вызвать оператор копирования/муванья или деструктор, класс обращается к `man`,
который благодаря шаблонному параметру помнит какой тип сейчас лежит в `any`.

С помощью такого подхода также можно легко проверять лежит ли сейчас в `any` условный тип `int`.
`this->man == &value_manager<int>`. Такая техника используется для реализации `any_cast`.
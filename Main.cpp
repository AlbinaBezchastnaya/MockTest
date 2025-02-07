#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

class Iconnection // Абстрактный класс-интерфейс
{
protected:
    string alert; // Переменная для хранения имитируемых значений
public:
    Iconnection() {}
    virtual ~Iconnection() {}
    virtual void open() = 0;                    // Выполнить соединение с БД
    virtual void execQuery(string command) = 0; // Выполнить действие над базой данных
    virtual void close() = 0;                   // Выполнить рассоединение с базой данных
    virtual const string& getAlert() const { return alert; } // Метод для получения значения alert
};

class DBConnection : public Iconnection // Тестируемый класс
{
protected:
    string alert = "0";
public:
    void open() override                // Выполнить соединение с БД
    {
        bool descriptor = true; // Имитация получения дескриптора соединения
        if (!descriptor)
        {
            alert = "Error: can't create descriptor";
        }
        else
        {
            alert = "Success!"; // Имитация cout << "Success!"
        }
        if (true)               // Имитация подключенмя к серверу
        {
            alert = "Error: can't connect to database"; // Имитация cout << "Error: can't connect to database"
        }
        else {
            alert = "Success!"; // Имитация cout << "Success!"
        }
    }
    void execQuery(string command) override
    {
        alert = command;         // Имитация действий над базой данных
        //"CREATE TABLE IF NOT EXISTS TableName (...)"
    }

    void close() override
    {
        alert = "Connection closed";
    }
};

class classThatUsesDB
{
protected:
    Iconnection* dbConnection;
    string coutValue;            // Переменная для хранения значений, выводимых в консоль, а также действия в базе данных (имитация вывода в консоль и действия в базе данных)
public:
    classThatUsesDB(Iconnection* dbConnection) : dbConnection(dbConnection) {}

    void openConnection()
    {
        dbConnection->open();
    }
    void useConnection()
    {
        coutValue = "CREATE TABLE IF NOT EXISTS TableName (...)";
        dbConnection->execQuery(coutValue);
    }
    void closeConnection()
    {
        dbConnection->close();
        coutValue = "Connection closed";
    }

    const string& getCoutValue() const { return coutValue; }  // Метод для получения значения coutValue
};

void testBD(classThatUsesDB* classDB) // Алгоритм для исследования
{
    classDB->openConnection();
    classDB->useConnection();
    classDB->closeConnection();
}

class MockDB : public Iconnection     // Mock-класс
{
public:
    MOCK_METHOD(void, open, ());
    MOCK_METHOD(void, execQuery, (string));
    MOCK_METHOD(void, close, ());
};

TEST(classThatUsesDBTest, test1)
{
    MockDB mockDB;                    // Создать мок-объект

    classThatUsesDB item(&mockDB);    // Создать объект classThatUsesDB, используя мок-объект

    EXPECT_CALL(mockDB, open).Times(1);                  // Проверить выполнение функций
    EXPECT_CALL(mockDB, execQuery).Times(1);
    EXPECT_CALL(mockDB, close).Times(1);

    testBD(&item);                    // Выполнить тест

    ASSERT_EQ(item.getCoutValue(), "Connection closed"); // Проверить значение переменной
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
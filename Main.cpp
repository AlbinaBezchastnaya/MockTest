#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace testing;

class Iconnection // ����������� �����-���������
{
protected:
    string alert; // ���������� ��� �������� ����������� ��������
public:
    Iconnection() {}
    virtual ~Iconnection() {}
    virtual void open() = 0;                    // ��������� ���������� � ��
    virtual void execQuery(string command) = 0; // ��������� �������� ��� ����� ������
    virtual void close() = 0;                   // ��������� ������������� � ����� ������
    virtual const string& getAlert() const { return alert; } // ����� ��� ��������� �������� alert
};

class DBConnection : public Iconnection // ����������� �����
{
protected:
    string alert = "0";
public:
    void open() override                // ��������� ���������� � ��
    {
        bool descriptor = true; // �������� ��������� ����������� ����������
        if (!descriptor)
        {
            alert = "Error: can't create descriptor";
        }
        else
        {
            alert = "Success!"; // �������� cout << "Success!"
        }
        if (true)               // �������� ����������� � �������
        {
            alert = "Error: can't connect to database"; // �������� cout << "Error: can't connect to database"
        }
        else {
            alert = "Success!"; // �������� cout << "Success!"
        }
    }
    void execQuery(string command) override
    {
        alert = command;         // �������� �������� ��� ����� ������
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
    string coutValue;            // ���������� ��� �������� ��������, ��������� � �������, � ����� �������� � ���� ������ (�������� ������ � ������� � �������� � ���� ������)
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

    const string& getCoutValue() const { return coutValue; }  // ����� ��� ��������� �������� coutValue
};

void testBD(classThatUsesDB* classDB) // �������� ��� ������������
{
    classDB->openConnection();
    classDB->useConnection();
    classDB->closeConnection();
}

class MockDB : public Iconnection     // Mock-�����
{
public:
    MOCK_METHOD(void, open, ());
    MOCK_METHOD(void, execQuery, (string));
    MOCK_METHOD(void, close, ());
};

TEST(classThatUsesDBTest, test1)
{
    MockDB mockDB;                    // ������� ���-������

    classThatUsesDB item(&mockDB);    // ������� ������ classThatUsesDB, ��������� ���-������

    EXPECT_CALL(mockDB, open).Times(1);                  // ��������� ���������� �������
    EXPECT_CALL(mockDB, execQuery).Times(1);
    EXPECT_CALL(mockDB, close).Times(1);

    testBD(&item);                    // ��������� ����

    ASSERT_EQ(item.getCoutValue(), "Connection closed"); // ��������� �������� ����������
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
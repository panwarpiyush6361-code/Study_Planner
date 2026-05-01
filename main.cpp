#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Task
{
private:
        string subject, topic, deadline, priority, status;

public:
        Task(string s = "", string t = "", string d = "", string p = "", string st = "Pending")
        {
                subject = s;
                topic = t;
                deadline = d; 
                priority = p;
                status = st;
        }

        
        string toFile()
        {
                return subject + "|" + topic + "|" + deadline + "|" + priority + "|" + status;
        }

        void display(int num)
        {
                cout << endl;
                cout << "*TASK no. :" << num << endl;
                cout << "+++++++++++++++++++++++++\n";
                cout << "Subject: " << subject << endl;
                cout << "Topic: " << topic << endl;
                cout << "Deadline: " << deadline << endl;
                cout << "Priority: " << priority << endl;
                cout << "Status: " << status << endl;
                cout << "++++++++++++++++++++++++++\n\n";
        }

        void markDone() { status = "Done"; }

        string getStatus() { return status; }
        string getPriority() { return priority; }
        string getSubject() { return subject; }
};

class Person
{
protected:
        string username;
};

class UserManagement : public Person
{
private:
        string password;

public:
        void signup()
        {
                cin.ignore();
                cout << "Enter username: ";
                getline(cin, username);

                cout << "Enter password: ";
                getline(cin, password);

                ofstream file("users.txt", ios::app);
                file << username << " " << password << endl;
        }

        bool login()
        {
                string u, p;
                cin.ignore();
                cout << endl;
                cout << "Enter username: ";
                getline(cin, username);

                cout << "Enter password: ";
                getline(cin, password);

                ifstream file("users.txt");

                while (file >> u >> p)
                {
                        if (u == username && p == password)
                                return true;
                }
                return false;
        }

        string getUser() { return username; }
};

class AnalysisBase
{
public:
        virtual void analysis()
        {
                cout << "Basic Analysis\n";
        }
};

class TaskManager : public AnalysisBase
{
private:
        string user;

public:
        TaskManager(string u)
        {
                user = u;
        }

        void addTask()
        {
                string s, t, d, p;

                cin.ignore();
                cout << "Enter subject: ";
                getline(cin, s);

                cout << "Enter topic: ";
                getline(cin, t);

                cout << "Enter deadline: ";
                getline(cin, d);

                cout << "Enter priority(1-low,2-medium,3-high): ";
                getline(cin, p);

                Task task(s, t, d, p);

                ofstream file("task.txt", ios::app);
                file << user << "|" << task.toFile() << endl;

                cout << "Task added!\n";
        }

        void viewTask()
        {
                ifstream file("task.txt");
                string line;
                int count = 0;

                while (getline(file, line))
                {
                        string u, s, t, d, p, st;
                        stringstream ss(line);

                        getline(ss, u, '|');
                        getline(ss, s, '|');
                        getline(ss, t, '|');
                        getline(ss, d, '|');
                        getline(ss, p, '|');
                        getline(ss, st, '|');

                        if (u == user)
                        {
                                count++;
                                Task task(s, t, d, p, st);
                                task.display(count);
                        }
                }
        }

        void markDone(int num)
        {
                ifstream file("task.txt");
                ofstream temp("temp.txt");

                string line;
                int count = 0;

                while (getline(file, line))
                {
                        string u, s, t, d, p, st;
                        stringstream ss(line);

                        getline(ss, u, '|');
                        getline(ss, s, '|');
                        getline(ss, t, '|');
                        getline(ss, d, '|');
                        getline(ss, p, '|');
                        getline(ss, st, '|');

                        if (u == user)
                        {
                                count++;
                                if (count == num)
                                {
                                        Task task(s, t, d, p, st);
                                        task.markDone();
                                        st = "Done";
                                }
                        }

                        temp << u << "|" << s << "|" << t << "|" << d << "|" << p << "|" << st << endl;
                }

                file.close();
                temp.close();
                remove("task.txt");
                rename("temp.txt", "task.txt");

                cout << "Task marked DONE\n";
        }

        void deleteTask(int num)
        {
                ifstream file("task.txt");
                ofstream temp("temp.txt");

                string line;
                int count = 0;

                while (getline(file, line))
                {
                        string u, s, t, d, p, st;
                        stringstream ss(line);

                        getline(ss, u, '|');
                        getline(ss, s, '|');
                        getline(ss, t, '|');
                        getline(ss, d, '|');
                        getline(ss, p, '|');
                        getline(ss, st, '|');

                        if (u == user)
                        {
                                count++;
                                if (count == num)
                                        continue;
                        }

                        temp << u << "|" << s << "|" << t << "|" << d << "|" << p << "|" << st << endl;
                }

                file.close();
                temp.close();
                remove("task.txt");
                rename("temp.txt", "task.txt");

                cout << "Task deleted\n";
        }

        void analysis() override
        {
                ifstream file("task.txt");
                string line;

                int total = 0, done = 0;
                string bestSubject = "";
                int bestPriority = 0;

                while (getline(file, line))
                {
                        string u, s, t, d, p, st;
                        stringstream ss(line);

                        getline(ss, u, '|');
                        getline(ss, s, '|');
                        getline(ss, t, '|');
                        getline(ss, d, '|');
                        getline(ss, p, '|');
                        getline(ss, st, '|');

                        if (u == user)
                        {
                                total++;

                                if (st == "Done")
                                        done++;

                                int pr = stoi(p);

                                if (st != "Done" && pr > bestPriority)
                                {
                                        bestPriority = pr;
                                        bestSubject = s;
                                }
                        }
                }

                cout << "\n===== ANALYSIS =====\n";
                cout << "Total: " << total << endl;
                cout << "Completed: " << done << endl;

                if (total > 0)
                        cout << "Progress: " << (done * 100) / total << "%\n";

                if (bestSubject == "")
                        cout << "All tasks completed!\n";
                else
                        cout << "Do this first: " << bestSubject << endl;
        }
};

void interface(string username)
{
        TaskManager t(username);
        int choice;

        while (true)
        {
                cout<<endl;
                cout << "------------------------------" << endl;
                cout << "1.ADD\n2.VIEW\n3.Mark_DONE\n4.DELETE\n5.ANALYSIS\n6.EXIT\n";
                cout << "------------------------------" << endl;
                cout << "Choice: ";
                cin >> choice;
                cout << endl;

                switch (choice)
                {
                case 1:
                        t.addTask();
                        break;

                case 2:
                        t.viewTask();
                        break;

                case 3:
                {
                        int n;
                        cout << "Task no: ";
                        cin >> n;
                        t.markDone(n);
                        break;
                }

                case 4:
                {
                        int n;
                        cout << "Task no: ";
                        cin >> n;
                        t.deleteTask(n);
                        break;
                }

                case 5:
                        t.analysis();
                        break;

                case 6:
                        return;

                default:
                        cout << "Invalid\n";
                }
        }
}

int main()
{
        UserManagement u;
        int ch;
        cout << "\n----------------------\n";
        cout << "1.Login\n2.Signup ";
        cout << "\n----------------------\n";
        cout << "choice : ";
        cin >> ch;

        if (ch == 1)
        {
                if (u.login())
                        interface(u.getUser());
                else
                        cout << "Login failed\n";
        }
        else
        {
                u.signup();
                interface(u.getUser());
        }

        return 0;
}
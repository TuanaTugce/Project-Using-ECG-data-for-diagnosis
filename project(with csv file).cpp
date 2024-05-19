#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

template<typename T>
class Node {
public:
    T data1;
    T data2;
    T data3;
    Node* next;

    Node()
    {
        data1 = T();
        data2 = T();
        data3 = T();
        next = nullptr;
    }

    Node(const T& d1, const T& d2, const T& d3, Node* n)
    {
        data1 = d1;
        data2 = d2;
        data3 = d3;
        next = n;
    }
};

template<typename T>
class MyLinkedList {
private:
    Node<T>* head = nullptr;
    int size = 0;

public:
    bool isEmpty() const
    {
        return head == nullptr;
    }
    void pushBack(Node<T>* newNode)
    {
        if (isEmpty())
            head = newNode;
        else
        {
            Node<T>* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    void clear()
    {
        while (!isEmpty())
        {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            size--;
        }
        if (size != 0)
            throw out_of_range("There is an error in clear function");
    }

    // Destructor
    ~MyLinkedList()
    {
        clear();
    }

    void print() const
    {
        Node<T>* temp = head;

        while (temp)
        {
            cout << temp->data1 << "   " << temp->data2 << "   " << temp->data3 << endl;
            temp = temp->next;
        }
        cout << endl;
    }
    void print(ofstream& fout) const
    {
        fout << head->data1 << "   " << head->data2 << "   " << head->data3 << endl;
    }

    void popFront()
    {
        if (isEmpty())
        {
            cout << "The list is already empty." << endl;
            return;
        }

        Node<T>* temp = head;
        head = head->next;

        if (temp)
        {
            delete temp;
            size--;
        }
        else
        {

            cout << "Error: Attempted to pop from an empty list." << endl;
        }
    }

    int findHeartBeat() const {
        if (isEmpty()) {
            throw out_of_range("Not enough elements in the list to calculate BPM.");
        }

        Node<T>* current = head;
        if (current->next == nullptr)
            return -1;

        Node<T>* nextNode = current->next;
        float bpm = 60.0 / (nextNode->data3 - current->data3);
        if (bpm <= 60)
            return 1;
        else if (bpm > 60 && bpm < 100)
            return 2;
        else
            return 3;
    }

    void sepWrite(ofstream& brady, ofstream& normal, ofstream& tachy)
    {
        if (!brady.is_open() || !tachy.is_open() || !normal.is_open())
        {
            cout << "An Error occured in sepWrite() function while opening files" << endl;
        }

        else
        {
            cout << "Files opened successfully" << endl;
            int last = 0;

            while (size>1)
            {
                switch (findHeartBeat())
                {
                case 1:
                    print(brady);
                    last = 1;
                    break;
                case 2:
                    print(normal);
                    last = 2;
                    break;
                case 3:
                    print(tachy);
                    last = 3;
                    break;
                }
                popFront();
            }

            switch (last)
            {
                case 1:
		    print(brady);
		    break;
                case 2:
                    print(normal);
                    break;
                case 3:
                    print(tachy);
		    break;
            }
            popFront();
            clear();

        }
    }

    void writeToList(ifstream& file)
    {
        if (!file.is_open())
        {
            cout << "An Error occured in writeToList function while opening files" << endl;
        }
        else
        {
            cout << "File opened successfully" << endl;

            string line;
            while (getline(file, line))
            {
                stringstream inputString(line);

                float data1, data2, data3;
                string tempString;

                getline(inputString, tempString, ';');
                data1 = atof(tempString.c_str());

                getline(inputString, tempString, ';');
                data2 = atof(tempString.c_str());

                getline(inputString, tempString, ';');
                data3 = atof(tempString.c_str());

                Node<float>* temp = new Node<float>(data1, data2, data3, nullptr);
                pushBack(temp);
            }
        }

    }

    void writeToFile(ofstream& file)
    {
        if(!file.is_open())
            throw out_of_range("An Error occured in writeToFile function while opening files");

        while (!isEmpty())
        {
            print(file);
            popFront();
        }
        clear();
    }

    void combine(ofstream& combinedfile, ifstream& file1, ifstream& file2)
    {
        if (!combinedfile.is_open() || !file1.is_open() || !file2.is_open())
        {
            cout << "An Error occured in combine function while opening files" << endl;
        }
        writeToList(file1);
        writeToFile(combinedfile);
        combinedfile << "***************************" << endl;
        writeToList(file2);
        writeToFile(combinedfile);

        clear();
    }
};


int main()
{
    MyLinkedList<float> list;

    ifstream file1("person1-TEST.csv");
    list.writeToList(file1);
    list.print();
    file1.close();

    ofstream brady1("person1.bradycardia.txt");
    ofstream normal1("person1.normal.txt");
    ofstream tachy1("person1.tachycardia.txt");

    list.sepWrite(brady1, normal1, tachy1);

    brady1.close();
    normal1.close();
    tachy1.close();
    list.clear();

    cout << endl << endl;

    ifstream file2("person2-TEST.csv");
    list.writeToList(file2);
    list.print();
    file2.close();

    ofstream brady2("person2.bradycardia.txt");
    ofstream normal2("person2.normal.txt");
    ofstream tachy2("person2.tachycardia.txt");

    list.sepWrite(brady2, normal2, tachy2);

    brady2.close();
    normal2.close();
    tachy2.close();

    ofstream combinedBrady("Bradycardia-Person-1-2.txt");
    ofstream combinedNormal("Normal-Person-1-2.txt");
    ofstream combinedTachy("Tachycardia-Person-1-2.txt");

    ifstream person1Brady("person1.bradycardia.txt");
    ifstream person2Brady("person2.bradycardia.txt");
    list.combine(combinedBrady, person1Brady, person2Brady);
    combinedBrady.close();
    person1Brady.close();
    person2Brady.close();

    ifstream person1Normal("person1.normal.txt");
    ifstream person2Normal("person2.normal.txt");
    list.combine(combinedNormal, person1Normal, person2Normal);
    combinedNormal.close();
    person1Normal.close();
    person2Normal.close();

    ifstream person1Tachy("person1.tachycardia.txt");
    ifstream person2Tachy("person2.tachycardia.txt");
    list.combine(combinedTachy, person1Tachy, person2Tachy);
    combinedTachy.close();
    person1Tachy.close();
    person2Tachy.close();

    return 0;
}

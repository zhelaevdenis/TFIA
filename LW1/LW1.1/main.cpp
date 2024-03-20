#include <iostream>
#include <fstream>
#include <vector>
/*
������ ������� ������ ��� ����, � ������ ������ ���������� ���������,
���������� � � ��� ��������, ������ ������ - ���������(��������� ���������� � 1),
����������� ������ ���������, � ������� ��� �������:
5 3 mur
s0/y1 s0/y2 s1/y1 s2/y2 s3/y1
1 2 5 4 3
3 1 3 2 4
2 5 1 5 2
*/

const std::string ERROR_OPENING_FILES{ "Error opening file!" };

struct pairOfnumb
{
    int value;
    int outputSignal;
};

int main()
{
    //��� ������ � �������
    std::ifstream FileInput("input.txt");
    std::ofstream FileOutput("output.txt");

    if (!FileInput.is_open() || !FileOutput.is_open())
    {
        std::cout << ERROR_OPENING_FILES << std::endl;
        return 1;
    }

    int n, m; //����������� �������
    FileInput >> n >> m;
    std::vector < std::vector <pairOfnumb> > table(m, std::vector <pairOfnumb>(n)); //���� �������
    std::vector <pairOfnumb> vecOfTops(n + 1);//������ ������
    std::string strToRead; //������ ��� ������ 
    int intToRead; //����� ��� ������
    char chToRead;


    FileInput >> strToRead;

    //��������� ������ ������
    for (int i = 1; i < n+1; i++)
    {
        FileInput >> chToRead >> intToRead >> chToRead;
        vecOfTops[i].value = i;
        //std::cout << vecOfTops[i].value << ' ';
        FileInput >> chToRead >> intToRead;
        vecOfTops[i].outputSignal = intToRead;
        //std::cout << vecOfTops[i].outputSignal << ' ';
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            FileInput >> intToRead;
            if (intToRead >= 0)
            {
                table[i][j].value = intToRead;
                table[i][j].outputSignal = vecOfTops[intToRead].outputSignal;
            }
            else
            {
                table[i][j].value = 0;
                table[i][j].outputSignal = 0;
            } 
        }
    }

    //������� ��������� ����������
    for (int i = 1; i < n+1; i++)
    {
        std::cout << ' ' << i << "  ";
        FileOutput << ' ' << i << "  ";
    }
    std::cout << std::endl;
    FileOutput << std::endl;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (table[i][j].value != 0)
            {
                std::cout << table[i][j].value << ';' << table[i][j].outputSignal << ' ';
                FileOutput << table[i][j].value << ';' << table[i][j].outputSignal << ' ';
            }
            else
            {
                std::cout << "_" << ';' << "_" << ' ';
                FileOutput << "_" << ';' << "_" << ' ';
            }
        }
        std::cout << std::endl;
        FileOutput << std::endl;
    }



    return 0;
}
// 

/*
/������ ������� ����
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            FileInput >> intToRead;
            table[i][j].value = intToRead;
            --intToRead;
            table[i][j].outputSignal = vecOfTops[intToRead].outputSignal;
        }
    }


    //������� ��������� ����������
    for (int i = 0; i < n; i++)
    {
        FileOutput << ' ' << i << "  ";
    }
    FileOutput << std::endl;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            FileOutput << table[i][j].value << ';' << table[i][j].outputSignal << ' ';
        }
        FileOutput << std::endl;
    }
*/


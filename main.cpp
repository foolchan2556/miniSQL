#include "Catalog.h"
#include "main.h"
#include "record.h"
#include "buffer.h"
#include "index.h"
#include "Interpreter.h"

int main()
{
	//��ӡ�����Ϣ
	cout << endl;
	cout << "\t\t***********************************************" << endl;
	cout << "\t\t             ��ӭʹ�� MiniSQL !" << endl;
	cout << "\t\t               Version (1.0)  " << endl;
	cout << "\t\t            " << endl;
	cout << "\t\t        copyright(2012) all right reserved !" << endl;
	cout << "\t\t***********************************************" << endl;
	cout << endl << endl;

	while(1)
	{
		cout << "MiniSQL-->> ";

		if(Interpreter() == 0) break;
	}
}

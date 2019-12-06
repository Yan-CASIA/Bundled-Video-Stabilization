/*
2018.11.16 �޸���const int max_dist = 20000;//������ݿ�Ƚ����⣬��ĺܶ࣬����Ҫ��ıȽϴ�֮ǰ�ر�׼������feature_loss ��һ��ܴ󣬴ﵽ��0.15
*/

//��ʹ�ñ������и���Ƶ��ÿһ֡��Ȼ��ʹ��matlab������ƽ�����ÿһ֡��Ȼ��ʹ��merge_frame_for_bundled������Ƶ,���ʹ��matlab�������ָ���ж�


#include <opencv2/opencv.hpp>
#include <process.h>  
#include <windows.h>  
#include <regex>
#include<io.h>
#include<direct.h>
using namespace cv;
using namespace std;


void getAllFiles(string path, vector<string>& files, string format)
{
	long long hFile = 0;//�ļ����  64λ��long ��Ϊ intptr_t
	struct _finddata_t fileinfo;//�ļ���Ϣ 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //�ļ�����
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//�ļ������в���"."��".."
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //�����ļ�����
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format); //�ݹ�����ļ���
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//��������ļ��У������ļ���
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}



int main(int argc, char* argv[])
{


	char* class_name[3] = { "Regular", "Crowd", "Parallax" };

	
	for (int cl = 1; cl < 3; cl++)
	{

		char filePath_unstable[40] = "E:\\data\\PROPOSED\\unstable\\";
		strcat(filePath_unstable, class_name[cl]);

		

		char path_videos[40];
		strcpy(path_videos, filePath_unstable);
		strcat(path_videos, "\\videos");

		//strcpy(filePath_unstable, "E:\\data\\stable\\");
		//strcat(filePath_unstable, cl.c_str());
		//char * filePath_unstable = strcat(, );
		////��ȡ��·���µ������ļ�  
		vector<string>files_unstable;
		string format = ".avi";				 //�����ļ��ĸ�ʽ
		//getAllFiles(filePath_stable, files_stable, format);
		getAllFiles(path_videos, files_unstable, format);

		//int numframe[] = { 256, 513, 492, 844,478, 248, 527, 528, 572, 597, 432 };
		for (int i = 0; i < files_unstable.size(); i++)
		{
			int the_start_num = files_unstable[i].find_last_of("\\");
			string path_temp = files_unstable[i].substr(the_start_num + 1);//, filename.length()-4);
			string path_cl = "E:\\data\\BUNDLED2\\images\\" + string(class_name[cl]) + "\\" + path_temp;

			

			VideoCapture capture;
			capture.open(string(files_unstable[i]));
			int numframes = capture.get(7);
			if (numframes > 1000)
				continue;
			if (0 != _access(path_cl.c_str(), 0))
			{
				// if this folder not exist, create a new one.
				_mkdir(path_cl.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
				//���� ::_mkdir  ::_access Ҳ�У���֪��ʲô��˼
			}
			Mat frame;
			int weishu = 1;
			while (numframes / 10 > 0)
			{
				weishu++;
				numframes = numframes / 10;

			}
			numframes = capture.get(7);
			Mat gray;
			int index = 1;
			for (int j = 0; j < numframes; j++)
			{
				capture >> frame;
				cvtColor(frame, gray, CV_RGB2GRAY);//�޳���ͷ��β����ȫ��ͼ�������matlab�������������������Ϊ�������
				if (frame.empty() || countNonZero(gray)==0)//
					continue;
				char ss[100];
				string temp = "%0" + to_string(weishu) + "d";
				sprintf(ss, temp.c_str(), index++);
				string sss = ss;
				
				
				cout << class_name[cl]<<"  "<<path_temp << "  " << j << "/" << numframes << endl;
				imwrite("E:\\data\\BUNDLED2\\images\\" + string(class_name[cl]) + "\\" + path_temp+"\\" + sss + ".png", frame);
				
			}


			capture.release();


		}
	}
	

	/*string path_videos = "E:\\Bundled\\results\\";

	VideoWriter writer_dense;
	writer_dense.open("bd.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, Size(640, 360), true);
	for (int i = 0; i < 246; i++)
	{
		string path = path_videos + to_string(i + 1) + ".bmp";
		Mat frame = imread(path);
		writer_dense << frame;
		cout << i << endl;
	}
	writer_dense.release();*/
	return 0;
}
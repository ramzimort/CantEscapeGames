///HEADER STUFF

#pragma once


struct Bone 
{
	//FOR NOW PUBLIC
	int index = -1;
	std::string name;
	bool updatedVQS;

	Matrix nodeTransformation;
	Matrix offsetMatrix;
	Matrix accumTransformation;
	Matrix vqs;

	std::string parent;
	std::vector<std::string> children;
};
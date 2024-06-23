#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

class Person 
{
public:
	std::string firstName;
	std::string lastName;
	std::unique_ptr<Person> son;
	std::unique_ptr<Person> daughter;

	Person(const std::string& firstName, const std::string& lastName) : firstName(firstName), lastName(lastName) {}

	void printFamily(int level = 0) const 
	{
		for (int i = 0; i < level; ++i) 
			std::cout << " ";
		
		std::cout << firstName << " " << lastName << std::endl;
		if (son) 
			son->printFamily(level + 1);
		
		if (daughter) 
			daughter->printFamily(level + 1);
		
	}
};
class FamilyTree 
{
public:
    std::unique_ptr<Person> root;

    Person* findPerson(const std::string& fullName)
    {
        if (!root)
            return nullptr;
        return findPersonHelper(root.get(), fullName); //get() Return value Pointer to the managed object or nullptr if no object is owned.
    }

    void addMember(const std::string& relation) 
    {
        // Пример строки: "Son of [Ilya Ivanovich] is [Ivan Ilyich]"
        std::string parentFullName, childFullName, relationType;
        try 
        {
            parseRelation(relation, parentFullName, childFullName, relationType);
        }
        catch (std::exception& e) 
        {
            std::cout << "Exception occured: " << e.what() << std::endl;
            return;
        }

        Person* parent = findPerson(parentFullName);
        if (!parent) 
        {
            std::cerr << "Parent not found: " << parentFullName << std::endl;
            return;
        }
        std::string child_first_name; // сюда сохраним имя нового ребенка (Ilya)
        std::string child_second_name; // сюда сохраним фамилию нового ребенка (Ilyich)
        splitFullName(childFullName, child_first_name, child_second_name);
        if (relationType == "Son") {
            parent->son = std::make_unique<Person>(child_first_name, child_second_name);
        }
        else if (relationType == "Daughter") {
            parent->daughter = std::make_unique<Person>(child_first_name, child_second_name);
        }
        else {
            throw std::invalid_argument("Invalid relation type: " + relationType);
        }
    }
    void printTree() const 
    {
        if (root)        
            root->printFamily();
        
    }

private:
    void parseRelation(const std::string& relation, std::string& parentFullName, std::string& childFullName, std::string& relationType) 
    {
        //1. Находит позиции открывающей и закрывающей скобок для имени родителя.
        //2. Находит позиции открывающей и закрывающей скобок для имени ребенка.
        //3. Извлекает имена родителя и ребенка из строки.
        //4. Находит позицию пробела после слова "of" и извлекает тип отношения.
        //Если какие - либо из этих шагов не удаются, метод выбрасывает исключение.
        std::size_t startOfParentName = relation.find('[');
        std::size_t endOfParentName = relation.find(']', startOfParentName + 1);
        if (startOfParentName == std::string::npos || endOfParentName == std::string::npos) 
            throw std::invalid_argument("Invalid relation format: " + relation);
        

        std::size_t startOfChildName = relation.find('[', endOfParentName + 1);
        std::size_t endOfChildName = relation.find(']', startOfChildName + 1);

        if (startOfChildName == std::string::npos || endOfChildName == std::string::npos) 
            throw std::invalid_argument("Invalid relation format: " + relation);        

        parentFullName = relation.substr(startOfParentName + 1, endOfParentName - startOfParentName - 1);
        childFullName = relation.substr(startOfChildName + 1, endOfChildName - startOfChildName - 1);

        std::size_t startOfRelationType = 0;
        std::size_t endOfRelationType = relation.find(' ', startOfRelationType);
        relationType = relation.substr(startOfRelationType, endOfRelationType - startOfRelationType);
    }

    Person* findPersonHelper(Person* node, const std::string& fullName) 
    {
        if (node->firstName == fullName) 
            return node;
        
        Person* result = nullptr;
        if (node->son) 
        {
            result = findPersonHelper(node->son.get(), fullName);
            if (result) 
                return result;
            
        }
        if (node->daughter) {
            result = findPersonHelper(node->daughter.get(), fullName);
            if (result) 
                return result;
            
        }
        return nullptr;
    }

    void splitFullName(const std::string& fullName, std::string& firstName, std::string& lastName) 
    {
        std::size_t spaceIndex = fullName.find(' ');
        if (spaceIndex == std::string::npos) {
            throw std::invalid_argument("Invalid full name: " + fullName);
        }
        firstName = fullName.substr(0, spaceIndex);
        lastName = fullName.substr(spaceIndex + 1);
    }
};
int main() 
{
	FamilyTree familyTree;
	familyTree.root = std::make_unique<Person>("Ilya", "Ivanovich");
	familyTree.addMember("Son of [Ilya Ivanovich] is [Ivan Ilyich]");
	familyTree.addMember("Daughter of [Ilya Ivanovich] is [Marusy Ivanovna]");
	familyTree.addMember("Son of [Ivan Ilyich] is [Petr Ivanovich]");
	familyTree.addMember("Daughter of [Ivan Ilyich] is [Anna Ivanovna]");
	familyTree.addMember("Son of [Marusy Ivanovna] is [Nikolay Petrovich]");
	familyTree.addMember("Daughter of [Marusy Ivanovna] is [Olga Petrovna]");
	familyTree.printTree();

	return 0;
}

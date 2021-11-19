## Morpheus
### Description
Morpheus is a simple library for make scenario tree

### Installing
- Recommended approach to use cmake FetchContent
- Morpheus CMakeLists.txt create static library, so after you call FetchContent_MakeAvailable CMake creates dependency,
so you can just add library to your executable by 'target_link_libraries' or by something else what you prefer

### Usage
Morpheus uses config file for create scenario tree. Config file contains description of scenario. Format:<br>
```
[nodeName]{body} -> [dependencies];
```
- `nodeName` - name of node. For example `begin`
- `body` - node data. This is a bit of difficult point. `body` can be simple string or user data.
If you use string you can write your text to `{}` without "". For example:<br> 
    ```
    [SomeStrNode]{Hello! This is the message that you can get in your program~~~} -> [Oh! Okey];
    ```
    Also, you can use your own data. Format a bit different:<br>
    ```
    [SomeUserNode]{message1="First message", message2="Second message"} -> [It's tricky!];
    ```
- 'dependencies' - sub nodes name

For parse config file use<br>
`morph::Loader<DataType>::parseFromFile("<scenario name>")`<br>
or <br>
`morph::Loader<DataType>::parseFromData("<scenario data>")`<br>
First for parsing file. Second for parsing std::string.
- DataType - your data type. You can use std::string or another types for which implemented operator = with std::string.
For your own types create class and inherit HasDataType with specifying template argument by your class name. You need 
to implement registerData method and create fields.
Example of user data type:<br>
    ```
    class Scenario : public morph::HasDataType<Scenario>
    {
    public:
        void registerData() override
        {
            setData(Mandatory::MANDATORY, "message1", &Scenario::message1);
            setData(Mandatory::MANDATORY, "message2", &Scenario::message2);
            setData(Mandatory::OPTIONAL, "message3", &Scenario::message3);
        }
        std::string message1;
        std::string message2;
        std::string message3;
    };
    ```
    If type mandatory and parser couldn't find it in body of node, it will throw exception
- Method `parseFromData` returns root node. For iterate over tree use `get`. Another methods you can check in Node.hpp
while documentation isn't written :)

### Example of usage
#### Config exaple
```
[begin]
{
message1 = "Message1 from begin",
message2 = "Message2 from begin"
} -> [branch1] [branch2];

[branch1]
{
message1 = "Message1 from branch1",
message2 = "Message2 from branch1",
message3 = "Message3 from branch1"
} -> [end];

[branch2]
{
message1 = "Message1 from branch2",
message2 = "Message2 from branch2"
} -> [end];

[end]
{
message1 = "End message",
message2 = "sub info",
message3 = "break"
};
```
### main.cpp example
```
int main()
{
	auto t = morph::Loader<Scenario>::parseFromFile("scenario1.cfg");
	std::string answer;
	while (true)
	{
		std::cout << "Message 1: " << t->value.message1 << "\n";
		std::cout << "Message 2: " << t->value.message2;
		if (!t->value.message3.empty())
		{
			std::cout << "\nMessage 3: " << t->value.message3;
		}
		std::cout << std::endl;
		if (!t->count()) break;
		std::cout << "Available answers: "<< t->getAvailableNodes() << std::endl;
		std::getline(std::cin, answer);
		auto buf = t->get(answer);
		if (buf == nullptr) std::cout << "Wrong answer! I can't continue dialogue" << std::endl;
		else t = buf;
	}
    return 0;
}
```

For user data was used previous `Scenario` class :)

If you have problems with library, you can leave issue

--[[
Lua only takes in floating point numbers.
Lua only takes up tp 13 decimal places.

Comment Section
-- - is the same as //

String Section
io.write("Content") - write to console.
io.read() - read whatever you type. Similar to C++ getchar().
# - size of the variable.
string.len(variable) - get the length of string.
# and .len returns same result.
\b - backspace
\t - tab
\\ - print backslash.
\" - print double quote.
\' - print single quote.
.. - add string with string.
type(variable) - check type of variable.
string.gsub(variable, "Replace this", "Replace with") - this will search the string for "Replace this" and overwrite it with "Replace with".
string.find(variable, "Content you searching for") - this will search the string for "Content you searching for" and return you the index of the first character.
string.upper(variable) - make all the characters uppercase.
string.lower(variable) - make all the characters lowercase.
string = multi-line comment characters will allow you to type inside and new line automatically. Whitespaces will be noted automatically.
string.format("I am preparing the next variable for 10 decimal places. %.10f"), 10.421415125 - string.format allows output to be prepared as intended.
%s - string
%.10f - 10 decimal places float.

Arithmetic Section
Lua does not support ++, --, +=, -=, /= and *=;
floor(number) - round down.
ceiling(number) - round up.
max(firstNum, secondNum) - prints the secondNum; higher number.
min(firstNum, secondNum) - prints the firstNum; lower number.
pow(number) - number to the power.
sqrt(number) - square root of number.
math.randomseed(os.time()) - same as srand(time(NULL)), to produce a different random every time the code is ran.
math.random() - produce a number between 0.0 to 0.999999...
math.random(min, max) - a random number will be produced between min and max.
math.pi - returns pi value.

Condition Section
> - greater than.
< - lesser than.
>= - greater than or equal to.
<= - lesser than or equal to.
== - is equal.
~= - not equal.
and - &&
or - ||
not - !
elseif() - else if()
if(condition) then - if(condition)
elseif(condition) then - else if (condition)
else - does not have the need to add then.
end - to be added at the end of every conditional statement to show that it is ended.
if statement in one line from C++ is - hello.length() >= 5 ? true : false
In Lua is - hello.length() >= 5 and true or false

Scope Section
local variableName - this will make variableName only available within a block.

Loop Section
while(condition) do - while(condition).
repeat - same as while loop, activates until the condition is false.
until - to be added below repeat so that it knows what condition to meet to stop.
for i = 0, 10, 1 do - for(i = 0; i <= 10; ++i)
break - same as C++.
end - to be added at the end of every conditional statement except repeat and until.
There is no continue statement.

Table Section - Array
variable = {} - Same as C++.

Conversion of types
tostring(variable) - std::to_string(variable)

]]

name = "Eugene"
message = [[This is a very long string.
It will automatically go the next line like how I typed in the script.
My name is ]]

message = message .. name
io.write("Size of ", message, ": ", #message, "\n", "Hello?", "\n", "\"")

io.write("Math section", "\n", "100 + 5 = ", 100+5, "\n", "100 / 4.5 = ", 100 / 4.5, '\n')

math.randomseed(os.time())
io.write("Random number betweeen 0 to 100: ", string.format("%.10f", math.random() + math.random(0, 100)), '\n')

randomNumber = math.random(0, 100)

if randomNumber >= 50 then
	io.write("Random number: ", randomNumber, " is greater or equal to 50.", '\n')
	local checkStatementAbove = randomNumber >= 50 and true or false
	io.write("Is the above statement true?: ", tostring(checkStatementAbove), '\n')
else
	io.write("Random number: ", randomNumber, " is lesser than 50.", '\n')
	local checkStatementAbove = randomNumber < 50 and true or false
	io.write("Is the above statement true?: ", tostring(checkStatementAbove), '\n')
end

message = [[I like Lua.
I like to eat sweet.
I like to program.]]

io.write(string.gsub(message, 'I', "We"), '\n')

i = 0;
while (i < 10) do
	io.write(tostring(i), '\n')
	i = i + 1
	if i == 5 then
		break 
		end
end

repeat
	io.write("Keep typing any numbers except 0: ", '\n')
	number = io.read()
	io.write("The number you entered is: ", number, '\n')
until tonumber(number) == 0

for i = 0, 10, 1 do
	io.write(i, '\n')
end

months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}

-- This is basically looping through each individual string in months array.
for word, month in pairs(months) do
	io.write(month, '\n')
end

io.write("Index 1 of months: ", months[1], '\n')

-- I rewrite everything from months to arrayMonths but started with 0 index. If you initialise months = {"Some value"}, it will start the index with 1.
arrayMonths = {}
for i = 0, 11, 1 do
	arrayMonths[i] = months[i + 1]
end

io.write("Index 0 of arrayMonths: ", arrayMonths[0], '\n');
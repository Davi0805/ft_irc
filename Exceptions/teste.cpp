#include "Exceptions.hpp"

void fun(void)
{
    throw ERR_NEEDMOREPARAMS("PASS");
}

int main()
{
    try{
        fun();
    }
    catch (const IRCException& e)
    {
        
    }
}
#include <iostream>
#include <vector>

#include "ast.hh"

using namespace std;
using namespace ast;

class Test {

    protected:
        virtual Program *makeProgram() = 0;
    public:
        void execute() {
            theProgram = makeProgram();
            cout << "executing " << theProgram->getName() << endl;
            theProgram->run();
            theProgram->getSymbolTable().print();
            delete theProgram;
        }
};

class Test_AssignmentStatement1 : public Test {
    protected:
        virtual Program *makeProgram() {
            Statement *s = new AssignmentStatement("x", new Num(10));
            return new Program("P1", s);
        }
};

class Test_AssignmentStatement2 : public Test {
    protected:
        virtual Program *makeProgram() {
            Statement *s1 = new AssignmentStatement("x", new Num(10));
            Statement *s2 = new AssignmentStatement("y", new Num(5));
            Statement *s3 = new AssignmentStatement("z", new AddExpression(new Var("x"), new Var("y")));
            SequenceStatement *slist = new SequenceStatement();
            slist->addStatement(s1);
            slist->addStatement(s2);
            slist->addStatement(s3);
            return new Program("P2", slist);
        }
};

/////////////////////////////////////////////////////////////////////////
class Test_IfStatmentTrue : public Test {
    protected:
        virtual Program *makeProgram() {
            Statement *s1 = new AssignmentStatement("x", new Num(10));
            Statement *s2 = new AssignmentStatement("y", new Num(5));
            Statement *s3 = new AssignmentStatement("z", new AddExpression(new Var("x"), new Var("y")));

            Expression *l = new Var("z");
            Expression *r = new Num(12);

            BoolExpression *b = new GreaterExpression(l, r);
            SequenceStatement *ifBody = new SequenceStatement();
            Statement *s4 = new AssignmentStatement("x", new Num(0));
            ifBody->addStatement(s4);
            Statement *if1 = new IfStatement(b, ifBody);

            SequenceStatement *slist = new SequenceStatement();
            slist->addStatement(s1);
            slist->addStatement(s2);
            slist->addStatement(s3);
            slist->addStatement(if1);
            return new Program("P3", slist); // x -> 0 (changed).
        }
};

class Test_IfStatmentFalse : public Test {
    protected:
        virtual Program *makeProgram() {
            Statement *s1 = new AssignmentStatement("x", new Num(10));
            Statement *s2 = new AssignmentStatement("y", new Num(5));
            Statement *s3 = new AssignmentStatement("z", new AddExpression(new Var("x"), new Var("y")));

            Expression *l = new Var("z");
            Expression *r = new Num(12);

            BoolExpression *b = new LesserExpression(l, r);
            SequenceStatement *ifBody = new SequenceStatement();
            Statement *s4 = new AssignmentStatement("x", new Num(0));
            ifBody->addStatement(s4);
            Statement *if1 = new IfStatement(b, ifBody);

            SequenceStatement *slist = new SequenceStatement();
            slist->addStatement(s1);
            slist->addStatement(s2);
            slist->addStatement(s3);
            slist->addStatement(if1);
            return new Program("P4", slist); // x -> 10 (unchanged).
        }
};

class Test_While : public Test {
    protected:
        virtual Program *makeProgram() {
            Statement *s1 = new AssignmentStatement("s", new Num(0));
            Statement *s2 = new AssignmentStatement("n", new Num(1));

            Expression *l = new Var("n");
            Expression *r = new Num(5);

            BoolExpression *b = new LesserExpression(l, r);
            SequenceStatement *whileBody = new SequenceStatement();
            Statement *s3 = new AssignmentStatement("s", new AddExpression(new Var("s"), new Var("n")));
            Statement *s4 = new AssignmentStatement("n", new AddExpression(new Var("n"), new Num(1)));
            whileBody->addStatement(s3);
            whileBody->addStatement(s4);
            Statement *while1 = new WhileStatement(b, whileBody);

            SequenceStatement *slist = new SequenceStatement();
            slist->addStatement(s1);
            slist->addStatement(s2);
            slist->addStatement(while1);
            return new Program("P5", slist); // x -> 10 (unchanged).
        }
};

/////////////////////////////////////////////////////////////////////////


int main() {
    cout << "testing AST ..." << endl;
    Test_AssignmentStatement1 t1;
    Test_AssignmentStatement2 t2;
    Test_IfStatmentTrue t3;
    Test_IfStatmentFalse t4;
    Test_While t5;
    vector<Test*> testcases{&t1, &t2, &t3, &t4, &t5};
    for(auto& t : testcases) {
	    t->execute();
    }
    return 0;
}

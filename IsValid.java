import java.util.Stack;

public class IsValid {
    public static void main(String[] args){
        System.out.println(isValid("({})"));
        System.out.println(isValid("([}}])"));
        return ;
    }
    public static boolean isValid(String s){
        if(s.length()%2 != 0||s.charAt(0)==')'||s.charAt(0)==']'||s.charAt(0)=='}') return false;
        Stack<Character> stack = new Stack<Character>();
        for(int i=0; i<s.length();i++){
            switch (s.charAt(i)){
                case ')':
                    if(stack.empty())return false;
                    if(stack.peek()=='(')stack.pop();
                    else return false;
                    break;
                case '}':
                    if(stack.empty())return false;
                    if(stack.peek()=='{')stack.pop();
                    else return false;
                    break;
                case ']':
                    if(stack.empty())return false;
                    if(stack.peek()=='[')stack.pop();
                    else return false;
                    break;
                default:
                    stack.push(s.charAt(i));
                    break;
            }
        }
        return stack.empty();
    }
}

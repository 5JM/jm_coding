public class MergeSortTwoLists {
    //Definition for singly-linked list.
    public static class ListNode {
        int val;
        ListNode next;
        ListNode() {}
        ListNode(int val) { this.val = val; }
        ListNode(int val, ListNode next) { this.val = val; this.next = next; }
     }
     public static void main(String[] args){
        ListNode l1=new ListNode(1,new ListNode(2,new ListNode(4)));
        ListNode l2=new ListNode(1,new ListNode(3,new ListNode(4)));
        ListNode re = mergeTwoLists(l1,l2);
        while(re!=null){
            System.out.println(re.val);
            re=re.next;
        }
        return;
     }
    public static ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        ListNode result=new ListNode(0,null);
        ListNode current = result;
        while(l1!=null&&l2!=null){
            if(l1.val<= l2.val){
                current.next=l1;
                l1=l1.next;
            }else{
                current.next=l2;
                l2=l2.next;
            }
            current=current.next;
        }
        current.next= (l1!=null)?l1:l2;
        return result.next;
    }
}

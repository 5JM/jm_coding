public class addTwoNums {
    /**
     * Definition for singly-linked list.
     * public class ListNode {
     *     int val;
     *     ListNode next;
     *     ListNode() {}
     *     ListNode(int val) { this.val = val; }
     *     ListNode(int val, ListNode next) { this.val = val; this.next = next; }
     * }
     */
    static class ListNode{
        int val;
        ListNode next;
        ListNode(){}
        ListNode(int val){this.val=val;}
        ListNode(int val, ListNode next) { this.val = val; this.next = next; }
    }
    static ListNode addTwoNumbers(ListNode l1, ListNode l2){
        int carry=0;
        ListNode dumHead = new ListNode(0);
        ListNode p=l1,q=l2,cur=dumHead;

        while (p!=null || q!=null){
            int x = (p!=null)?p.val : 0;
            int y = (q!=null)?q.val : 0;
            int sum = carry+x+y;
            carry=sum/10;
            cur.next=new ListNode(sum%10);
            cur=cur.next;
            if(p!=null) p = p.next;
            if(q!=null) q = q.next;
        }
        if(carry>0)
            cur.next=new ListNode(carry);
        return dumHead.next;
    }
}

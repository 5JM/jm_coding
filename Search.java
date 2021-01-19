import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;

public class Search {
    private static ArrayList<Integer>[] tree = new ArrayList[16];
    private static boolean[] isVisited;
    private static StringBuilder sb;

    private static final String SPACE = " ";
    public static void main(String[] args){
        treeInit();
        sb=new StringBuilder();
        isVisited = new boolean[tree.length];
        BFS(1);
        System.out.println(sb);

        sb=new StringBuilder();
        isVisited=new boolean[tree.length];
        DFS(1);
        System.out.println(sb);
        return;
    }
    //tree 초기화, tree[i]에 자식노드 인덱스 저장
    private static void treeInit() {
        for(int i = 1; i < 8; i++) {
            tree[i] = new ArrayList<>();
            //if(i > 8) continue;

            int sonNode = i * 2;
            for(int adder = 0; adder < 2; adder++)
                tree[i].add(sonNode + adder);
        }
    }
    static public void BFS(int start){
        Queue<Integer> q = new LinkedList<>();
        q.offer(start);

        isVisited[start] = true;
        sb.append(start).append(SPACE);

        while(!q.isEmpty()){
            int current = q.poll();
            if(tree[current]==null||tree[current].size()==0)continue;
            for(int next:tree[current]){
                if(isVisited[next]) continue;
                isVisited[next]=true;

                sb.append(next).append(SPACE);
                q.offer(next);
            }
        }
    }
    //깊이우선탐색
    static public void DFS(int current){
        if(isVisited[current]) return;
        isVisited[current] = true;
        sb.append(current).append(SPACE);
        if(tree[current]==null||tree[current].size()==0) return;
        for(int next:tree[current]){
            if(isVisited[next])
                continue;
            DFS(next);
        }
    }
}

import java.awt.Point;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;


/*
* This is a game commonly found at Cracker Barrel. It comes in the shape of a triangle and has 14 pegs with 15 possible
* holes to put them in. The goal is to hop each neighboring peg until there is only one peg left. You can only hop a peg
* over pegs directly next to it and into an open space.
*
* Positions :
*
*   11  12  13  14  15
*     7   8   9   10
*       4   5   6
*         2   3
*           1
*
* This program will take the position of the empty spot and return the set of moves needed to win the game.
* */

public class CrackerBarrel {
    // True = peg, False = empty
    private static boolean[][] board;
    
    // Up Left, Up Right, Down Right, Down Left, Left, Right
    private static final int[] DX = { -2, -2, 2, 2, 0, 0 };
    private static final int[] DY = { 0, 2, 0, -2, -2, 2 };
    
    // This function takes the position of the peg, initializes the board, and calls the overloaded function.
    public static ArrayList<Integer> solve( int n ) {
        if( n < 1 || n > 15 ) return null;
        
        init(n);
        return solve(new ArrayList<Integer>());
    }
    
    // This function uses recursion and back-tracking to find the order the pegs should be hopped.
    private static ArrayList<Integer> solve( ArrayList<Integer> sequence ) {
        // As the ArrayList stores the start and end position of a moving peg, the size will be 2 * (14 - 1).
        if( sequence.size() == 26 ) return sequence;
        
        // Stores the positions of pegs on the board.
        Queue<Point> pegs = new LinkedList<>();
        for( int i = 0; i < 5; i++ )
            for( int j = 0; j < board[i].length; j++ )
                if( board[i][j] )
                    pegs.add(new Point(i, j));
        
        while( !pegs.isEmpty() ) {
            Point p = pegs.poll();
            
            // Tries to make the peg hop in every direction.
            for( int i = 0; i < DX.length; i++ ) {
                // Check if it can move a peg in that direction without going out of bounds.
                if( p.x + DX[i] > 4 || p.x + DX[i] < 0 ) continue;
                if( p.y + DY[i] > board[p.x + DX[i]].length - 1 || p.y + DY[i] < 0 ) continue;
                
                // Check if there is a peg to hop over, and a space to hop to.
                if( !board[p.x + DX[i] / 2][p.y + DY[i] / 2] ) continue;
                if( board[p.x + DX[i]][p.y + DY[i]] ) continue;
                
                // Invert the pegs effected by the hop.
                board[p.x + DX[i]][p.y + DY[i]]
                        = true;
                board[p.x + DX[i] / 2][p.y + DY[i] / 2] = false;
                board[p.x][p.y] = false;
                
                // Add the start and end position of the moving peg.
                sequence.add(pointToPos(p));
                sequence.add(pointToPos(new Point(p.x + DX[i], p.y + DY[i])));
                
                // Recursively call the function with the new board.
                solve(sequence);
                if( sequence.size() == 26 ) return sequence;
                
                // Revert to the state before the changes were made if the sequence failed.
                board[p.x + DX[i]][p.y + DY[i]] = false;
                board[p.x + DX[i] / 2][p.y + DY[i] / 2] = true;
                board[p.x][p.y] = true;
                sequence.remove(sequence.size() - 1);
                sequence.remove(sequence.size() - 1);
            }
        }
        return sequence;
    }
    
    // Creates a 2D array of 5 arrays with lengths descending from 5.
    public static void init( int n ) {
        board = new boolean[5][];
        for(int i = 0; i < 5; i++) {
            board[i] = new boolean[5 - i];
            Arrays.fill(board[i], true);
        }
        
        flip(n);
    }
    
    // Formatted output of the result.
    public static void print(ArrayList<Integer> result) {
        System.out.println("------------");
        for(int i = 0; i < result.size(); i++) {
            if(i % 2 == 0)
                System.out.printf("| %-2d -> ", result.get(i));
            else
                System.out.printf("%2d |\n------------\n", result.get(i));
        }
    }
    
    // Turns a point into a position on the board.
    private static int pointToPos( Point p ) {
        switch( p.x ) {
            case 0:
                return p.y + 11;
            case 1:
                return p.y + 7;
            case 2:
                return p.y + 4;
            case 3:
                return p.y + 2;
            default:
                return 1;
        }
    }
    
    // Inverts the boolean value at the given position.
    private static void flip( int n ) {
        if( n > 10 )
            board[0][n - 11] = !board[0][n - 11];
        else if( n > 6 )
            board[1][n - 7] = !board[1][n - 7];
        else if( n > 3 )
            board[2][n - 4] = !board[2][n - 4];
        else if( n > 1 )
            board[3][n - 2] = !board[3][n - 2];
        else
            board[4][0] = !board[4][0];
    }
    
    public static void main( String[] args ) throws IOException {
        System.out.println("11  12  13  14  15");
        System.out.println("  7   8   9   10");
        System.out.println("    4   5   6");
        System.out.println("      2   3");
        System.out.println("        1");
        System.out.println("\nPlease enter the number where the peg is missing.");
        
        Scanner in = new Scanner(System.in);
        
        int n = in.nextInt();
        if( n < 1 || n > 15 ) {
            System.out.println("Invalid Input!");
            return;
        }
        print(solve(n));
        
        System.in.read();
    }
}
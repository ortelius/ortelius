/*
 *
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.openmakesoftware.XDR;

public class Encryption
{

    // Most of the Makros ported as static class variables...
    private static int   N                = 624;

    private static int   M                = 397;

    private static int   MATRIX_A         = 0x9908b0df; /* L constant vector a */

    private static int   UPPER_MASK       = 0x80000000; /*
                                                         * most significant w-r
                                                         * bits
                                                         */

    private static int   LOWER_MASK       = 0x7fffffff; /*
                                                         * least significant r
                                                         * bits
                                                         */

    private static int   TEMPERING_MASK_B = 0x9d2c5680;

    private static int   TEMPERING_MASK_C = 0xefc60000;

    // private static long UnsignedIntMaxValue = 4294967296L;

    private static int[] mt               = null;      /*
                                                         * the array for the
                                                         * state vector
                                                         */

    private static int   mti              = N + 1;     /*
                                                         * if sgenrand() has not
                                                         * been called,
                                                         */

    public static void sgenrand(int seed)
    {
        /* setting initial seeds to mt[N] using */
        /* the generator Line 25 of Table 1 in */
        /* [KNUTH 1981, The Art of Computer Programming */
        /* Vol. 2 (2nd Ed.), pp102] */

        mt = new int[N];
        mt[0] = seed;
        // System.out.printf( "mt[0] = %d\n", mt[0] );
        for (mti = 1; mti < N; mti++)
        {
            mt[mti] = (69069 * mt[mti - 1]); // & 0xffffffffL; //%
                                                // UnsignedIntMaxValue;
            // System.out.printf( "mt[%d] = %d\n", mti, mt[mti] );
        }
    }

    static int genrand()
    {
        int y;

        final int[] mag01 = new int[] { 0x0, MATRIX_A };
        /* mag01[x] = x * MATRIX_A for x=0,1 */

        if (mti >= N)
        { /* generate N words at one time */
            int kk;

            if (mti == N + 1) /* if sgenrand() has not been called, */
                sgenrand(4357); /* a default initial seed is used */
            for (kk = 0; kk < N - M; kk++)
            {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + M] ^ (y >>> 1) ^ mag01[(int) (y & 0x1)];
                // System.out.printf( "y = %08x; %08x; %08x; mt[%d] = %08x\n",
                // y, (y >>> 1), mag01[(int) (y & 0x1)], kk, mt[kk] );
            }

            for (; kk < N - 1; kk++)
            {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + (M - N)] ^ (y >>> 1) ^ mag01[(int) (y & 0x1)];
            }

            y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
            mt[N - 1] = mt[M - 1] ^ (y >>> 1) ^ mag01[(int) (y & 0x1)];
            mti = 0;
        }

        y = mt[mti++];
        y ^= (y >>> 11);// TEMPERING_SHIFT_U(y)
        y ^= (y << 7) & TEMPERING_MASK_B; // TEMPERING_SHIFT_S(y)
        y ^= (y << 15) & TEMPERING_MASK_C; // TEMPERING_SHIFT_T(y)
        y ^= (y >>> 18); // TEMPERING_SHIFT_L(y)

        return y;
    } // END RANDOM NUMBER GENERATING CODE

    public static void Encrypt(byte[] Buffer)
    {
        // Encrypts the buffer so that its contents cannot be
        // easily determined by network sniffers etc.
        for (int k = 0; k < Buffer.length; k++)
        {
            int rand = genrand();
            // System.out.print("\nEncrypting " + Buffer[k] + " with "
            // + (rand & 255) + " => ");
            Buffer[k] = (byte) (Buffer[k] ^ (rand & 255));
            // System.out.print(Buffer[k]);
        }
    }

    public static void Encrypt(byte[] Buffer, int offset)
    {
        // Encrypts the buffer so that its contents cannot be
        // easily determined by network sniffers etc.
        for (int k = offset; k < Buffer.length; k++)
        {
            int rand = genrand();
            // System.out.print("\nEncrypting " + Buffer[k] + " with "
            // + (rand & 255) + " => ");
            Buffer[k] = (byte) (Buffer[k] ^ (rand & 255));
            // System.out.print(Buffer[k]);
        }
    }

    public static void Encrypt(byte[] Buffer, int from, int to)
    {
        // System.out.println( "Encryption.Encrypt( [], " + from + ", " + to + "
        // )" );
        // Encrypts the buffer so that its contents cannot be
        // easily determined by network sniffers etc.
        for (int k = from; k < to; k++)
        {
            int rand = genrand();
            // System.out.print("Encrypting " + Buffer[k] + " with "
            // + (rand & 255) + " => ");
            Buffer[k] = (byte) (Buffer[k] ^ (rand & 255));
            // System.out.println(Buffer[k]);
        }
    }

    public static void Decrypt(byte[] Buffer)
    {
        // Decrypts buffer contents
        for (int k = 0; k < Buffer.length; k++)
        {
            Buffer[k] = (byte) (Buffer[k] ^ (genrand() & 255));
        }
    }

    public static void Decrypt(byte[] Buffer, int offset)
    {
        // Decrypts buffer contents
        for (int k = offset; k < Buffer.length; k++)
        {
            Buffer[k] = (byte) (Buffer[k] ^ (genrand() & 255));
        }
    }

    public static void Decrypt(byte[] Buffer, int from, int to)
    {
        // Decrypts buffer contents
        for (int k = from; k < to; k++)
        {
            int rand = genrand();
            // System.out.print("Decrypting " + Buffer[k] + " with "
            // + (rand & 255) + " => ");
            Buffer[k] = (byte) (Buffer[k] ^ (rand & 255));
            // System.out.println(Buffer[k]);
        }
    }

    public static void main(String[] args)
    {
        sgenrand(1234);
        for(int n = 1; n <= 10; n++) {
            System.out.println(n + ": " + genrand());
        }
    }
}

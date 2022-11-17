/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package dmadmin.model;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;
import java.util.UUID;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;
import io.jsonwebtoken.Jwts;

public class JWTGenerateValidateRSA {

    public static void main(String[] args) throws InvalidKeySpecException, NoSuchAlgorithmException {

        String jwt = createJwtSignedHMAC(0,UUID.randomUUID().toString());
        System.out.println(jwt);

        Jws<Claims> token = parseJwt(jwt);

        System.out.println(token.getBody());
    }

    public static Jws<Claims> parseJwt(String jwtString) throws InvalidKeySpecException, NoSuchAlgorithmException {

        PublicKey publicKey = getPublicKey();

        Jws<Claims> jwt = Jwts.parser()
                .setSigningKey(publicKey)
                .parseClaimsJws(jwtString);

        return jwt;
    }


    public static String createJwtSignedHMAC(int userid, String uuid) throws InvalidKeySpecException, NoSuchAlgorithmException {

        PrivateKey privateKey = getPrivateKey();

        String jwtToken = Jwts.builder()
                .setSubject(Integer.toString(userid))
                .setId(uuid)
                .signWith(privateKey)
                .compact();

        return jwtToken;
    }

    public static String readFileAsString(String filename)
    {
     String newString = "";
     try
     {
      String nextLine = "";
      BufferedReader br;
      br = new BufferedReader(new FileReader(filename));
      StringBuffer sb = new StringBuffer();
      while ((nextLine = br.readLine()) != null)
      {
       if (!nextLine.contains("KEY-----"))
         sb.append(nextLine);
      }
      br.close();
      // remove newlines
      newString = sb.toString().replaceAll("\\R+", "");
     }
     catch (IOException e)
     {
      // TODO Auto-generated catch block
      e.printStackTrace();
     }

     return newString;
    }

    private static PublicKey getPublicKey() throws NoSuchAlgorithmException, InvalidKeySpecException {
        String rsaPublicKey = "";
        
        String pubkey = "/opt/deployhub/keys/id_rsa.pub";
        
        if (System.getenv("JwtPublicKey") != null)
         pubkey = System.getenv("JwtPublicKey");
        
        if (new File(pubkey).isFile())
         rsaPublicKey = readFileAsString(pubkey);
          
        rsaPublicKey = rsaPublicKey.replace("-----BEGIN PUBLIC KEY-----", "");
        rsaPublicKey = rsaPublicKey.replace("-----END PUBLIC KEY-----", "");
        
        System.out.println("PUB");
        System.out.println(rsaPublicKey);
        
        X509EncodedKeySpec keySpec = new X509EncodedKeySpec(Base64.getDecoder().decode(rsaPublicKey));
        KeyFactory kf = KeyFactory.getInstance("RSA");
        PublicKey publicKey = kf.generatePublic(keySpec);
        return publicKey;
    }

    private static PrivateKey getPrivateKey() throws NoSuchAlgorithmException, InvalidKeySpecException {
        String rsaPrivateKey = "";

        String prikey = "/opt/deployhub/keys/id_rsa";
        
        if (System.getenv("JwtPrivateKey") != null)
         prikey = System.getenv("JwtPrivateKey");
        
        if (new File(prikey).isFile())
         rsaPrivateKey = readFileAsString(prikey);
        
        rsaPrivateKey = rsaPrivateKey.replace("-----BEGIN PRIVATE KEY-----", "");
        rsaPrivateKey = rsaPrivateKey.replace("-----END PRIVATE KEY-----", "");

        System.out.println("PRIV");
        System.out.println(rsaPrivateKey);
        
        PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(Base64.getDecoder().decode(rsaPrivateKey));
        KeyFactory kf = KeyFactory.getInstance("RSA");
        PrivateKey privKey = kf.generatePrivate(keySpec);
        return privKey;
    }
}

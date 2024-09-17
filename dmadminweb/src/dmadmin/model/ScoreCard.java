package dmadmin.model;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

import org.apache.commons.collections.ArrayStack;
import org.json.JSONArray;
import org.json.JSONObject;

public class ScoreCard
{
 private static final String SCORECARD_API_BASE_URL = "https://api.securityscorecards.dev/projects/";

 public String commit_sha;
 public boolean pinned;
 public float Score;
 public float Maintained;
 public float CodeReview;
 public float CIIBestPractices;
 public float License;
 public float SignedReleases;
 public float DangerousWorkflow;
 public float Packaging;
 public float TokenPermissions;
 public float BranchProtection;
 public float BinaryArtifacts;
 public float PinnedDependencies;
 public float SecurityPolicy;
 public float Fuzzing;
 public float SAST;
 public float Vulnerabilities;
 public float CITests;
 public float Contributors;
 public float DependencyUpdateTool;
 public float SBOM;
 public float Webhooks;

 public ScoreCard(String commit_sha, boolean pinned, float score, float Maintained, float CodeReview, float CIIBestPractices, float License, float SignedReleases, float DangerousWorkflow, float Packaging, float TokenPermissions, float BranchProtection, float BinaryArtifacts,
   float PinnedDependencies, float SecurityPolicy, float Fuzzing, float SAST, float Vulnerabilities, float CITests, float Contributors, float SBOM, float Webhooks, float DependencyUpdateTool)
 {
  this.commit_sha = commit_sha;
  this.pinned = pinned;
  this.Score = score;
  this.Maintained = Maintained;
  this.CodeReview = CodeReview;
  this.CIIBestPractices = CIIBestPractices;
  this.License = License;
  this.SignedReleases = SignedReleases;
  this.DangerousWorkflow = DangerousWorkflow;
  this.Packaging = Packaging;
  this.TokenPermissions = TokenPermissions;
  this.BranchProtection = BranchProtection;
  this.BinaryArtifacts = BinaryArtifacts;
  this.PinnedDependencies = PinnedDependencies;
  this.SecurityPolicy = SecurityPolicy;
  this.Fuzzing = Fuzzing;
  this.SAST = SAST;
  this.Vulnerabilities = Vulnerabilities;
  this.CITests = CITests;
  this.Contributors = Contributors;
  this.DependencyUpdateTool = DependencyUpdateTool;
  this.SBOM = SBOM;
  this.Webhooks = Webhooks;
 }

 public static ScoreCard getSecurityScore(String repoUrl, String commitSha) throws IOException
 {
  if (repoUrl == null || repoUrl.isEmpty())
  {
   return new ScoreCard(null, false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  }

  String githubUrl = repoUrl.replaceAll("git\\+ssh://git@", "").replaceAll("git\\+https://", "").replaceAll("http://", "").replaceAll("https://", "").replaceAll("git:", "").replaceAll("git\\+", "").replaceAll(".git", "");

  String fullUrl = SCORECARD_API_BASE_URL + githubUrl;

  if (commitSha != null && !commitSha.isEmpty())
  {
   fullUrl += "?commit=" + commitSha;
  }

  URL url = new URL(fullUrl);
  HttpURLConnection connection = (HttpURLConnection) url.openConnection();
  connection.setRequestMethod("GET");

  int statusCode = connection.getResponseCode();
  if (statusCode == 200)
  {
   Scanner scanner = new Scanner(connection.getInputStream());
   StringBuilder responseBuilder = new StringBuilder();
   while (scanner.hasNextLine())
   {
    responseBuilder.append(scanner.nextLine());
   }
   scanner.close();

   String responseJson = responseBuilder.toString();
   // Parse JSON response to extract score
   JSONObject jsonObject = new JSONObject(responseJson);
   float score = 0.0f;
   float Maintained = 0.0f;
   float CodeReview = 0.0f;
   float CIIBestPractices = 0.0f;
   float License = 0.0f;
   float SignedReleases = 0.0f;
   float DangerousWorkflow = 0.0f;
   float Packaging = 0.0f;
   float TokenPermissions = 0.0f;
   float BranchProtection = 0.0f;
   float BinaryArtifacts = 0.0f;
   float PinnedDependencies = 0.0f;
   float SecurityPolicy = 0.0f;
   float Fuzzing = 0.0f;
   float SAST = 0.0f;
   float Vulnerabilities = 0.0f;
   float CITests = 0.0f;
   float Contributors = 0.0f;
   float DependencyUpdateTool = 0.0f;
   float SBOM = 0.0f;
   float Webhooks = 0.0f;

   score = jsonObject.getFloat("score");

   JSONArray checksArray = jsonObject.getJSONArray("checks");

   // Iterate through the checks array
   for (int i = 0; i < checksArray.length(); i++)
   {
    JSONObject checkObj = checksArray.getJSONObject(i);
    String checkName = checkObj.getString("name");
    float checkscore = checkObj.getFloat("score");

    // Use switch statement to assign scores based on checkName
    switch (checkName)
    {
     case "Maintained":
      Maintained = checkscore;
      break;
     case "Code-Review":
      CodeReview = checkscore;
      break;
     case "CII-Best-Practices":
      CIIBestPractices = checkscore;
      break;
     case "License":
      License = checkscore;
      break;
     case "Signed-Releases":
      SignedReleases = checkscore;
      break;
     case "Dangerous-Workflow":
      DangerousWorkflow = checkscore;
      break;
     case "Packaging":
      Packaging = checkscore;
      break;
     case "Token-Permissions":
      TokenPermissions = checkscore;
      break;
     case "Branch-Protection":
      BranchProtection = checkscore;
      break;
     case "Binary-Artifacts":
      BinaryArtifacts = checkscore;
      break;
     case "Pinned-Dependencies":
      PinnedDependencies = checkscore;
      break;
     case "Security-Policy":
      SecurityPolicy = checkscore;
      break;
     case "Fuzzing":
      Fuzzing = checkscore;
      break;
     case "SAST":
      SAST = checkscore;
      break;
     case "Vulnerabilities":
      Vulnerabilities = checkscore;
      break;
     case "CI-Tests":
      CITests = checkscore;
      break;
     case "Contributors":
      Contributors = checkscore;
      break;
     case "Dependency-Update-Tool":
      DependencyUpdateTool = checkscore;
      break;
     case "SBOM":
      SBOM = checkscore;
      break;
     case "Webhooks":
      Webhooks = checkscore;
      break;
    }
   }
   return new ScoreCard(commitSha, true, score, Maintained, CodeReview, CIIBestPractices, License, SignedReleases, DangerousWorkflow, Packaging, TokenPermissions, BranchProtection, BinaryArtifacts, PinnedDependencies, SecurityPolicy, Fuzzing, SAST, Vulnerabilities, CITests, Contributors, SBOM, Webhooks, DependencyUpdateTool);
  }
  else
  {
   // Retry without commitSha if the first attempt fails
   fullUrl = SCORECARD_API_BASE_URL + githubUrl;
   url = new URL(fullUrl);
   connection = (HttpURLConnection) url.openConnection();
   connection.setRequestMethod("GET");

   statusCode = connection.getResponseCode();
   if (statusCode == 200)
   {
    Scanner scanner = new Scanner(connection.getInputStream());
    StringBuilder responseBuilder = new StringBuilder();
    while (scanner.hasNextLine())
    {
     responseBuilder.append(scanner.nextLine());
    }
    scanner.close();

    String responseJson = responseBuilder.toString();
    // Parse JSON response to extract score
    JSONObject jsonObject = new JSONObject(responseJson);
    float score = 0.0f;
    float Maintained = 0.0f;
    float CodeReview = 0.0f;
    float CIIBestPractices = 0.0f;
    float License = 0.0f;
    float SignedReleases = 0.0f;
    float DangerousWorkflow = 0.0f;
    float Packaging = 0.0f;
    float TokenPermissions = 0.0f;
    float BranchProtection = 0.0f;
    float BinaryArtifacts = 0.0f;
    float PinnedDependencies = 0.0f;
    float SecurityPolicy = 0.0f;
    float Fuzzing = 0.0f;
    float SAST = 0.0f;
    float Vulnerabilities = 0.0f;
    float CITests = 0.0f;
    float Contributors = 0.0f;
    float DependencyUpdateTool = 0.0f;
    float SBOM = 0.0f;
    float Webhooks = 0.0f;

    score = jsonObject.getFloat("score");

    JSONArray checksArray = jsonObject.getJSONArray("checks");

    // Iterate through the checks array
    for (int i = 0; i < checksArray.length(); i++)
    {
     JSONObject checkObj = checksArray.getJSONObject(i);
     String checkName = checkObj.getString("name");
     float checkscore = checkObj.getFloat("score");

     // Use switch statement to assign scores based on checkName
     switch (checkName)
     {
      case "Maintained":
       Maintained = checkscore;
       break;
      case "Code-Review":
       CodeReview = checkscore;
       break;
      case "CII-Best-Practices":
       CIIBestPractices = checkscore;
       break;
      case "License":
       License = checkscore;
       break;
      case "Signed-Releases":
       SignedReleases = checkscore;
       break;
      case "Dangerous-Workflow":
       DangerousWorkflow = checkscore;
       break;
      case "Packaging":
       Packaging = checkscore;
       break;
      case "Token-Permissions":
       TokenPermissions = checkscore;
       break;
      case "Branch-Protection":
       BranchProtection = checkscore;
       break;
      case "Binary-Artifacts":
       BinaryArtifacts = checkscore;
       break;
      case "Pinned-Dependencies":
       PinnedDependencies = checkscore;
       break;
      case "Security-Policy":
       SecurityPolicy = checkscore;
       break;
      case "Fuzzing":
       Fuzzing = checkscore;
       break;
      case "SAST":
       SAST = checkscore;
       break;
      case "Vulnerabilities":
       Vulnerabilities = checkscore;
       break;
      case "CI-Tests":
       CITests = checkscore;
       break;
      case "Contributors":
       Contributors = checkscore;
       break;
      case "Dependency-Update-Tool":
       DependencyUpdateTool = checkscore;
       break;
      case "SBOM":
       SBOM = checkscore;
       break;
      case "Webhooks":
       Webhooks = checkscore;
       break;
     }
    }
    return new ScoreCard(commitSha, false, score, Maintained, CodeReview, CIIBestPractices, License, SignedReleases, DangerousWorkflow, Packaging, TokenPermissions, BranchProtection, BinaryArtifacts, PinnedDependencies, SecurityPolicy, Fuzzing, SAST, Vulnerabilities, CITests, Contributors, SBOM, Webhooks, DependencyUpdateTool);
   }
   else
   {
    String githubToken = System.getenv("GITHUB_TOKEN");
    if (githubToken != null && !githubToken.isEmpty() && githubUrl.contains("github.com") && !commitSha.isEmpty())
    {
     try
     {
      List<String> command = Arrays.asList("scorecard", "--repo=" + githubUrl, "--commit=" + commitSha, "--format", "json");

      ProcessBuilder processBuilder = new ProcessBuilder(command);
      Process process = processBuilder.start();
      BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
      StringBuilder output = new StringBuilder();
      String line;
      while ((line = reader.readLine()) != null)
      {
       output.append(line);
      }
      int exitCode = process.waitFor();
      if (exitCode == 0)
      {
       System.out.println("SCORECARD: " + String.join(" ", command));
       JSONObject jsonObject = new JSONObject(output.toString());
       float score = 0.0f;
       float Maintained = 0.0f;
       float CodeReview = 0.0f;
       float CIIBestPractices = 0.0f;
       float License = 0.0f;
       float SignedReleases = 0.0f;
       float DangerousWorkflow = 0.0f;
       float Packaging = 0.0f;
       float TokenPermissions = 0.0f;
       float BranchProtection = 0.0f;
       float BinaryArtifacts = 0.0f;
       float PinnedDependencies = 0.0f;
       float SecurityPolicy = 0.0f;
       float Fuzzing = 0.0f;
       float SAST = 0.0f;
       float Vulnerabilities = 0.0f;
       float CITests = 0.0f;
       float Contributors = 0.0f;
       float DependencyUpdateTool = 0.0f;
       float SBOM = 0.0f;
       float Webhooks = 0.0f;

       score = jsonObject.getFloat("score");

       JSONArray checksArray = jsonObject.getJSONArray("checks");

       // Iterate through the checks array
       for (int i = 0; i < checksArray.length(); i++)
       {
        JSONObject checkObj = checksArray.getJSONObject(i);
        String checkName = checkObj.getString("name");
        float checkscore = checkObj.getFloat("score");

        // Use switch statement to assign scores based on checkName
        switch (checkName)
        {
         case "Maintained":
          Maintained = checkscore;
          break;
         case "Code-Review":
          CodeReview = checkscore;
          break;
         case "CII-Best-Practices":
          CIIBestPractices = checkscore;
          break;
         case "License":
          License = checkscore;
          break;
         case "Signed-Releases":
          SignedReleases = checkscore;
          break;
         case "Dangerous-Workflow":
          DangerousWorkflow = checkscore;
          break;
         case "Packaging":
          Packaging = checkscore;
          break;
         case "Token-Permissions":
          TokenPermissions = checkscore;
          break;
         case "Branch-Protection":
          BranchProtection = checkscore;
          break;
         case "Binary-Artifacts":
          BinaryArtifacts = checkscore;
          break;
         case "Pinned-Dependencies":
          PinnedDependencies = checkscore;
          break;
         case "Security-Policy":
          SecurityPolicy = checkscore;
          break;
         case "Fuzzing":
          Fuzzing = checkscore;
          break;
         case "SAST":
          SAST = checkscore;
          break;
         case "Vulnerabilities":
          Vulnerabilities = checkscore;
          break;
         case "CI-Tests":
          CITests = checkscore;
          break;
         case "Contributors":
          Contributors = checkscore;
          break;
         case "Dependency-Update-Tool":
          DependencyUpdateTool = checkscore;
          break;
         case "SBOM":
          SBOM = checkscore;
          break;
         case "Webhooks":
          Webhooks = checkscore;
          break;
        }
       }
       return new ScoreCard(commitSha, false, score, Maintained, CodeReview, CIIBestPractices, License, SignedReleases, DangerousWorkflow, Packaging, TokenPermissions, BranchProtection, BinaryArtifacts, PinnedDependencies, SecurityPolicy, Fuzzing, SAST, Vulnerabilities, CITests, Contributors, SBOM, Webhooks, DependencyUpdateTool);
      }
     }
     catch (IOException | InterruptedException e)
     {

     }
    }
   }
  }

  return new ScoreCard(null, false, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
 }
}

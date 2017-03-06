action demo1
{
  using stream $attachment
  {
    echo 'This is a file attachment.';
  }

  notify(notifier: 'email',
         to: ${environment.owner},
         subject: 'Test Notification',
         from: 'dm2@trinem.com',
         attachment: $attachment)
  {
    echo "Dear ${environment.owner.name},";
    echo '';
    echo "This is a test email message from DM2 on ${TRIDM_PLATFORM} to say";
    echo "that the application ${application.name} has been deployed to ";
    echo "the environment ${environment.name}";
    echo '';
    echo 'Best regards,';
    echo '';
    echo '     Deployment Manager 2';
    
    attachment(name: 'attachment2.txt', body: $attachment);

    attachment(name: 'attachment3.txt')
    {
      echo 'This is the third file attachment';
    }
    
    attachment(name: 'attachment4.txt')
    {
      testactionvbs(arg1: 'hello', arg2: 'world');
    }
  }
}

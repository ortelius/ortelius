action demo2
{
  notify(notifier: 'sms', to: ${environment.owner}, from: 'Trinem')
  {
    echo "Application ${application.name} has been deployed to environment ${environment.name}";
  }
}

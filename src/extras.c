  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */


  // if(strstr(hostname, "http://") != NULL){
  //   strncpy(temp_url, hostname+7, sizeof(temp_url));
  //   point_break = replace_w_space(temp_url);
  //   sscanf(point_break, "%s %s %s", hostname, port, path);
  //   // printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  // } else if (strstr(hostname, "https://") != NULL) {
  //   strncpy(temp_url, hostname+8, sizeof(temp_url));
  //   // printf("tEMPURL >>>> %s", temp_url);
  //   point_break = replace_w_space(temp_url);
  //   sscanf(point_break, "%s %s %s", hostname, port, path);
  //   // printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  // } else {
  //   point_break = replace_w_space(temp_url);
  //   sscanf(point_break, "%s %s %s", hostname, port, path);
  //   // printf("hostname >> %s, port >>> %s, path >> %s\n", hostname, port, path);
  // }

  char *replace_w_space(char *needs_space){
  // try strcmp?
  for (int i=0; i<strlen(needs_space); i++){
    if(needs_space[i] == "/" || needs_space[i] == ":" ){
      needs_space[i] = " ";
    } 
  }
  return needs_space;
}
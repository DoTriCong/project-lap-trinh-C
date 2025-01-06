#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
struct Date{
	int month, day, year;
};
struct Category{
	char CategoryId[10];
	char CategoryName[10];
};
struct Product{
	char productId[10];
	char cateGory[10];
	char productName[10];
	int quantity;
	int price;
};
struct Product products[100];
struct Category category[10]={
	{"1"}, {"2"}, {"3"}, {"4"}, {"5"},
    {"6"}, {"7"}, {"8"}, {"9"}, {"10"}
};
int numberProduct = 0;
int numberCategory = 0;
int isCategoryExist(const char *categoryId) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(category[i].CategoryId, categoryId) == 0) {
            return 1;
        }
    }
    return 0;
}
bool checkAdminAccount(const char *email, const char *password) {
    FILE *file = fopen("admin_accounts.txt", "r");
    if (file == NULL) {
        printf("\tLoi: Khong the mo file admin_accounts.txt!\n");
        return false;
    }
    char storedEmail[100], storedPassword[100];
    while (fgets(storedEmail, sizeof(storedEmail), file) != NULL) {
        char *comma = strchr(storedEmail, ',');
        if (comma == NULL) continue;
        *comma = '\0';
        strcpy(storedPassword, comma + 1);
        storedPassword[strcspn(storedPassword, "\n")] = '\0'; 
        storedEmail[strcspn(storedEmail, "\n")] = '\0';
        if (strcmp(email, storedEmail) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false; 
}
bool loginAccount() {
    char email[100], password[100];
    printf("\n\n\tDANG NHAP ADMIN\n");
    printf("\t=====================\n");
    printf("\tEmail: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';
    printf("\tPassword: ");
    int indexPass = 0;
     while (1) {
        c = getch(); 
        if (c == '\r') { 
            break;
        } else if (c == '\b') { 
            if (indexPass > 0){
                indexPass--;
                printf("\b \b"); 
            }
        } else if (indexPass < sizeof(password) - 1) { 
            password[indexPass++] = c;
            printf("*"); 
        }
    }
    password[indexPass] = '\0'; 
    if (checkAdminAccount(email, password)) {
        printf("\n\tDang nhap thanh cong!\n");
        return true;
    } else {
        printf("\n\tSai email hoac mat khau!\n");
        return false;
    }
}
//tao tep quan tri neu chua ton tai 
//email va password admin: admin@gmail.com,password123  
void createAdminFileIfNotExists() {
     FILE *file = fopen("admin_accounts.txt", "r");
    if (file == NULL) { 
        file = fopen("admin_accounts.txt", "w");
        if (file != NULL) {
            // Them mot tai khoan vao tep
            fprintf(file, "admin@gmail.com,password123\n");
            printf("\tTao tep admin_accounts.txt thanh cong voi tai khoan mac dinh!\n");
        } else {
            printf("\tLoi: Khong the tao tep admin_accounts.txt\n");
        }
    } else {
        printf("\tTep admin_accounts.txt da ton tai.\n");
    }
    fclose(file);
}
// Ham kiem tra tai khoan dang nhap
int validateLogin(const char *email, const char *password) {
    FILE *file = fopen("admin_accounts.txt", "r");
    if (file == NULL) {
        printf("Loi: Tep admin_accounts.txt khong ton tai.\n");
        return 0;
    }
    char line[256];
    char storedEmail[128], storedPassword[128];
    while (fgets(line, sizeof(line), file)) {
        // Tach email va password tu dong doc duoc
        if (sscanf(line, "%127[^,],%127s", storedEmail, storedPassword) == 2) {
            storedPassword[strcspn(storedPassword, "\n")] = '\0';
            // So sanh email va password
            if (strcmp(email, storedEmail) == 0 && strcmp(password, storedPassword) == 0) {
                fclose(file);
                return 1; 
            }
        }
    }
    fclose(file);
    return 0;
}
void printCategory() {
    printf("\tDanh sach danh muc:\n");
    printf("\t+-----+-------------------+-------------------+\n");
    printf("\t| STT | Ma danh muc       | Ten danh muc      |\n"); 
    printf("\t+-----+-------------------+-------------------+\n");
    int stt=1; 
    for (int i = 0; i < 10; i++) {
        if (strlen(category[i].CategoryId) > 0) {
            printf("\t| %-3d | %-17s | %-17s |\n", 
                   stt++, 
                   category[i].CategoryId, 
                   category[i].CategoryName);
        }
    }
    printf("\t+-----+-------------------+-------------------+\n");
}
// ham luu danh muc vao file
void saveCategoryToFile() {
    FILE *file = fopen("category.txt", "w");
    if (file == NULL) {
        printf("\tLoi: Khong the mo file!\n");
        return;
    }
    int hasData = 0;
    for (int i = 0; i < 10; i++) {
        if (strlen(category[i].CategoryId) > 0) {
            hasData = 1;
            break;
        }
    }
    if (!hasData) {
        printf("\tKhong co danh muc nao de luu!\n");
        fclose(file);
        return;
    }
    for (int i = 0; i < 10; i++) {
        if (strlen(category[i].CategoryId) > 0) {
            if (fprintf(file, "%s,%s\n", category[i].CategoryId, category[i].CategoryName) < 0) {
                printf("\tLoi: Khong the ghi vao file!\n");
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
    printf("\tDanh muc da duoc luu vao file.\n");
}
void addCategory() {
    for (int i = 0; i < 10; i++) {
        if (strlen(category[i].CategoryId) == 0) {
            printf("\tThem danh muc moi:\n");
            int valid = 0;
            while (!valid) {
                printf("\tID danh muc (toi da 5 ky tu): ");
                fgets(category[i].CategoryId, sizeof(category[i].CategoryId), stdin);
                fflush(stdin);  
                category[i].CategoryId[strcspn(category[i].CategoryId, "\n")] = '\0'; 
                if (strlen(category[i].CategoryId) == 0) {
                    printf("\tLoi: ID danh muc khong duoc de trong!\n");
                } else if (strlen(category[i].CategoryId) > 5) {
                    printf("\tLoi: ID danh muc qua dai!\n");
                } else {
                    int isDuplicate = 0;
                    for (int j = 0; j < i; j++) {
                        if (strcmp(category[i].CategoryId, category[j].CategoryId) == 0) {
                            printf("\tLoi: ID danh muc da ton tai!\n");
                            isDuplicate = 1;
                            break;
                        }
                    }
                    if (!isDuplicate) {
                        valid = 1; 
                    }
                }
            }
            valid = 0;
            while (!valid) {
                printf("\tTen danh muc (toi da 9 ky tu: ");
                fgets(category[i].CategoryName, sizeof(category[i].CategoryName), stdin);
                category[i].CategoryName[strcspn(category[i].CategoryName, "\n")] = '\0';
                //kiem tra tinh hop le cua ten danh muc 
                if (strlen(category[i].CategoryName) == 0) {
                    printf("\tLoi: Ten danh muc khong duoc de trong!\n");
                } else if (strlen(category[i].CategoryName) > 9) {
                    printf("\tLoi: Ten danh muc qua dai, vui long nhap toi da 9 ky tu!\n");
                } else {
                    int isDuplicate = 0;
                    for (int j = 0; j < i; j++) {
                        if (strcmp(category[i].CategoryName, category[j].CategoryName) == 0) {
                            printf("\tLoi: Ten danh muc da ton tai!\n");
                            isDuplicate = 1;
                            break;
                        }
                    }
                    if (!isDuplicate) {
                        valid = 1; 
                    }
                }
            }
            printf("\tThem danh muc thanh cong!\n");
            saveCategoryToFile();
            return;
        }
    }
    printf("\tDanh sach danh muc da day, khong the them moi!\n");
}
// ham tai danh muc
void loadCategoryFromFile() {
    FILE *file = fopen("category.txt", "r");
    if (file == NULL) {
        printf("\tLoi: Khong the mo file!\n");
        return;
    }
    int index = 0;
    while (fscanf(file, "%[^,],%[^\n]\n", category[index].CategoryId, category[index].CategoryName) == 2) {
        index++;
        if (index >= 10) break;
    }
    fclose(file);
    printf("\tDanh muc da duoc tai len tu file.\n");
}
// Ham chinh sua danh muc
void editCategory() {
    char x[30];
    int found = 0;
    printf("\tNhap ID danh muc can sua: ");
    while (getchar() != '\n');
    fgets(x, sizeof(x), stdin);
    x[strcspn(x, "\n")] = '\0';
    for (int i = 0; i < 10; i++) {
        if (strcmp(x, category[i].CategoryId) == 0) {
            found = 1;
            printf("\tThong tin danh muc hien tai:\n");
            printf("\tID: %s\n", category[i].CategoryId);
            printf("\tTen: %s\n", category[i].CategoryName);
            printf("\tNhap thong tin sua doi:\n");
//            printf("\tID danh muc moi: ");
//            fgets(category[i].CategoryId, sizeof(category[i].CategoryId), stdin);
//            category[i].CategoryId[strcspn(category[i].CategoryId, "\n")] = '\0';
            printf("\tTen danh muc moi: ");
            fgets(category[i].CategoryName, sizeof(category[i].CategoryName), stdin);
            category[i].CategoryName[strcspn(category[i].CategoryName, "\n")] = '\0';
            printf("\tSua thong tin danh muc thanh cong!\n");
            break;
        }
    }
    if (!found) {
        printf("\tKhong tim thay ID danh muc!\n");
    } else {
        saveCategoryToFile();
    }
}
// ham xoa danh muc
void deleteCategory(){
	char x[30];
	int found = 0;
	char a[30];
	printf("\tHay nhap ID muon xoa: ");
	while(getchar() != '\n');
	fgets(x, sizeof(x),stdin);
	x[strcspn(x, "\n")] = '\0';
	for (int i = 0; i < 10; i++) {
        if (strcmp(x, category[i].CategoryId) == 0) {
            found = 1;
            break;
        }
    }
    if(!found) {
    	printf("\tKhong tim thay ID\n");
    	return;
	}
	printf("\tBan co chac chan muon xoa (yes/no): ");
	fgets(a, sizeof(a),stdin);
	a[strcspn(a, "\n")] = '\0';
	
	if(strcmp(a, "yes") == 0){
		for (int i = 0; i < 10; i++) {
        if (strcmp(x, category[i].CategoryId) == 0) {
            found = 1;
            for(int j = i; j < 10; j++){
            	category[j] = category[j + 1];
			}
		strcpy(category[9].CategoryId, "");
        strcpy(category[9].CategoryName, "");
        printf("\tDa xoa danh muc thanh cong!\n");
        break;
        }
    }
    saveCategoryToFile();
    printCategory();
  }
  else{
  	printf("\tHuy thao tac xoa\n");
  }
}
//ham tim kiem danh muc theo ten
void searchCategory(){
	char a[30];
	int found = 0;
	printf("\tMoi ban nhap ten danh muc muon tim: ");
	getchar();
	fgets(a, sizeof(a), stdin);
	a[strcspn(a, "\n")] = '\0';
	for(int i = 0; i < 10; i++){
		if(strcmp(a, category[i].CategoryName) == 0){
			found = 1;
			printf("\tDa tim thay !\n");
			printf("\t+-----+-------------------+-------------------+\n");
    		printf("\t| STT | Ma danh muc       | Ten danh muc      |\n"); 
    		printf("\t+-----+-------------------+-------------------+\n");
    			for (int j = i; j < 9 ; j++) {
        			if (strlen(category[i].CategoryId) > 0) {
            			printf("\t| %-3d | %-17s | %-17s |\n", 
                   		i + 1, 
                   		category[i].CategoryId, 
                   		category[i].CategoryName);
            printf("\t+-----+-------------------+-------------------+\n");
			break;
          }
      }
	}
  }
      if(!found) {
      	printf("\tKhong tim thay!\n");
	}
}
// ham sap xep danh muc
void arrangeCategory() {
    char a[10];
    printf("\tHay chon cach sap xep theo ten (tang / giam): ");
    while(getchar() != '\n');
    fgets(a, sizeof(a), stdin);
    a[strcspn(a, "\n")] = '\0';
    if (strcmp(a, "tang") == 0 || strcmp(a, "giam") == 0) {
        for (int i = 0; i < 9; i++) {
            for (int j = i + 1; j < 10; j++) {
                if ((strcmp(a, "tang") == 0 && strcmp(category[i].CategoryName, category[j].CategoryName) > 0) ||
                    (strcmp(a, "giam") == 0 && strcmp(category[i].CategoryName, category[j].CategoryName) < 0)) {
                    struct Category temp = category[i];
                    category[i] = category[j];
                    category[j] = temp;
                }
            }
        }
    }
	else{
        printf("Lua chon khong hop le. Vui long thu lai.\n");
        return;
    }
    saveCategoryToFile();
}
// ham menu danh muc
void menuCategory() {
    int choice;
    do {
        printf("\t***QUAN LY DANH MUC***\n");
        printf("\t===========================\n");
        printf("\t[1]. Hien thi danh sach danh muc\n");
        printf("\t[2]. Them danh muc moi\n");
        printf("\t[3]. Chinh sua danh muc\n");
        printf("\t[4]. Xoa danh muc\n");
        printf("\t[5]. Tim kiem danh muc\n");
        printf("\t[6]. Sap xep danh muc\n");
        printf("\t[0]. Quay lai menu chinh\n");
        printf("\t===========================\n");
        printf("\tNhap lua chon: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                printf("\tQuay lai menu chinh.\n");
                break;
            case 1:
                printCategory();
                break;
            case 2:
                addCategory();
                break;
            case 3:
            	editCategory();
            	break;
            case 4:
            	deleteCategory();
            	break;
            case 5:
            	searchCategory();
            	break;
            case 6:
            	arrangeCategory();
            	break;
            default:
                printf("\tLua chon khong hop le!\n");
        }
    } while (choice != 0);
}
// ham tai san pham
void loadProductFromFile() {
    FILE *file = fopen("product.txt", "r");
    if (file == NULL) {
        printf("\tLoi: khong the mo file!\n");
        return;
    }
    numberProduct = 0;
    while (fscanf(file, "%[^,],%[^,],%[^,],%d,%d\n", 
                  products[numberProduct].productId, 
                  products[numberProduct].productName, 
                  products[numberProduct].cateGory, 
                  &products[numberProduct].quantity, 
                  &products[numberProduct].price) == 5) {
        numberProduct++;
        if (numberProduct >= 100) {
            printf("\tDanh sach san pham da day!\n");
            break;
        }
    }
    fclose(file);
    printf("\tSan pham da duoc tai len tu file.\n");
}
// ham luu san pham vao file
void saveProductToFile() {
    FILE *file = fopen("product.txt", "w");
    if (file == NULL) {
        printf("\tLoi: khong the mo file!\n");
        return;
    }
    for (int i = 0; i < numberProduct; i++) {
        fprintf(file, "%s,%s,%s,%d,%d\n", 
                products[i].productId, 
                products[i].productName, 
                products[i].cateGory, 
                products[i].quantity, 
                products[i].price);
    }
    fclose(file);
    printf("\tSan pham da duoc luu vao file.\n");
}
// ham them san pham
void addProduct() {
    if (numberProduct >= 100) {
        printf("\tDanh sach san pham da day!\n");
        return;
    }
    struct Product newProduct;
     while (getchar() != '\n' && !feof(stdin)); 
    printf("\tNhap Ma SP: ");
    fgets(newProduct.productId, sizeof(newProduct.productId), stdin);
    newProduct.productId[strcspn(newProduct.productId, "\n")] = '\0';
    printf("\tNhap ten san pham : ");
    fgets(newProduct.productName, sizeof(newProduct.productName), stdin);
    newProduct.productName[strcspn(newProduct.productName, "\n")] = '\0';
    printf("\tNhap Ma Danh Muc: ");
    fgets(newProduct.cateGory, sizeof(newProduct.cateGory), stdin);
    newProduct.cateGory[strcspn(newProduct.cateGory, "\n")] = '\0';
       if (!isCategoryExist(newProduct.cateGory)) {
        printf("\tLoi: Ma danh muc khong ton tai!\n");
        return;
    }
    printf("\tNhap So Luong: ");
    scanf("%d", &newProduct.quantity);
    getchar();
    if (newProduct.quantity <= 0) {
        printf("\tLoi: So luong khong hop le!\n");
        return;
    }
    printf("\tNhap Gia: ");
    scanf("%d", &newProduct.price);
    getchar();
    if (newProduct.price <= 0) {
        printf("\tLoi: Gia khong hop le!\n");
        return;
    }
    // Them san pham vao danh sach
    products[numberProduct] = newProduct;
    numberProduct++;
    printf("\tThem san pham thanh cong!\n");
    // Luu san pham vao file
    saveProductToFile();
}
// ham in danh sach san pham
void printProduct() {
    if (numberProduct == 0) {
        printf("\tDanh sach san pham rong!\n");
        return;
    }
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    printf("\t| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    for (int i = 0; i < numberProduct; i++) {
        printf("\t| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n", 
               i + 1, 
               products[i].productId, 
               products[i].productName, 
               products[i].cateGory,
               products[i].quantity, 
               products[i].price);
    }
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
}
//ham chinh sua san pham 
void editProduct() {
    char productId[20];
    int found = 0;
    // Nhap ma san pham can sua 
    printf("\tNhap Ma SP can chinh sua: ");
    getchar();
    fgets(productId, sizeof(productId), stdin);
    productId[strcspn(productId, "\n")] = '\0';
    // tim san pham trong danh sach 
    for (int i = 0; i < numberProduct; i++) {
        if (strcmp(products[i].productId, productId) == 0) {
            found = 1;
            //chinh sua thong tin san pham 
            printf("\tNhap Ten SP moi: ");
            fgets(products[i].productName, sizeof(products[i].productName), stdin);
            products[i].productName[strcspn(products[i].productName, "\n")] = '\0';
            printf("\tNhap Ma Danh Muc moi: ");
            fgets(products[i].cateGory, sizeof(products[i].cateGory), stdin);
            products[i].cateGory[strcspn(products[i].cateGory, "\n")] = '\0';
            // Kiem tra ma danh muc 
            int validCategory = 0;
            printf("\tDanh sach cac danh muc hien co: \n");
            for (int j = 0; j < 10; j++) {
                printf("\t- %s\n", category[j].CategoryId);  
                if (strcmp(category[j].CategoryId, products[i].cateGory) == 0) {
                    validCategory = 1;
                    break;
                }
            }
            if (!validCategory) {
                printf("\tLoi: Ma danh muc khong ton tai!\n");
                return;
            }
            printf("\tNhap So Luong moi: ");
            scanf("%d", &products[i].quantity);
            getchar();
            if (products[i].quantity <= 0) {
                printf("\tLoi: So luong khong hop le!\n");
                return;
            }
            printf("\tNhap Gia moi: ");
            scanf("%d", &products[i].price);
            getchar();
            if (products[i].price <= 0) {
                printf("\tLoi: Gia khong hop le!\n");
                return;
            }
            saveProductToFile();
            printf("\tChinh sua san pham thanh cong!\n");
            return;
        }
    }
    if (!found) {
        printf("\tLoi: San pham khong ton tai!\n");
    }
}
void deleteProduct() {
    char productId[20];
    int found = 0;
    printf("\tNhap Ma SP can xoa: ");
    getchar();
    fgets(productId, sizeof(productId), stdin);
    productId[strcspn(productId, "\n")] = '\0';
    for (int i = 0; i < numberProduct; i++) {
        if (strcmp(products[i].productId, productId) == 0) {
            found = 1;
            char confirm;
            printf("\tBan co chac chan muon xoa san pham nay? (y/n): ");
            scanf(" %c", &confirm);
            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < numberProduct - 1; j++) {
                    products[j] = products[j + 1];
                }
                numberProduct--;
                saveProductToFile();
                printf("\tSan pham da duoc xoa thanh cong!\n");
            } else {
                printf("\tHuy xoa san pham.\n");
            }
            return;
        }
    }
    if (!found) {
        printf("\tLoi: San pham khong ton tai!\n");
    }
}
void searchProductByName() {
    char searchName[50];
    int found = 0;
    printf("\tNhap Ten SP can tim kiem: ");
    getchar();
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    printf("\t| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    for (int i = 0; i < numberProduct; i++) {
        if (strstr(products[i].productName, searchName) != NULL) {
            found = 1;
            printf("\t| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n", 
                   i + 1, 
                   products[i].productId, 
                   products[i].productName, 
                   products[i].cateGory,
                   products[i].quantity, 
                   products[i].price);
        }
    }
    if (!found) {
        printf("\tKhong tim thay san pham nao!\n");
    } else {
        printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    }
}
void sortProductsByPrice() {
    int order;
    printf("\tChon thu tu sap xep:\n");
    printf("\t[1]. Tang dan\n");
    printf("\t[2]. Giam dan\n");
    printf("\tNhap lua chon: ");
    scanf("%d", &order);
    if (order != 1 && order != 2) {
        printf("\tLua chon khong hop le!\n");
        return;
    }
    // Sap xep san pham theo gia
    for (int i = 0; i < numberProduct - 1; i++) {
        for (int j = i + 1; j < numberProduct; j++) {
            if ((order == 1 && products[i].price > products[j].price) ||
                (order == 2 && products[i].price < products[j].price)) {
                struct Product temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
    // Hien thi danh sach sau khi sap xep
    printf("\tDanh sach san pham sau khi sap xep:\n");
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    printf("\t| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
    for (int i = 0; i < numberProduct; i++) {
        printf("\t| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n", 
               i + 1, 
               products[i].productId, 
               products[i].productName, 
               products[i].cateGory,
               products[i].quantity, 
               products[i].price);
    }
    printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
}
// Ham loc san pham theo danh muc hoac khoang gia
void filterProducts() {
    int choice;
    printf("\tCHON TIEU CHI LOC SAN PHAM\n");
    printf("\t[1]. Loc theo danh muc\n");
    printf("\t[2]. Loc theo gia tien\n");
    printf("\tNhap lua chon: ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        // Loc theo danh muc
        char categoryId[20];
        int foundCategory = 0;
        printf("\tNhap Ma Danh Muc: ");
        fgets(categoryId, sizeof(categoryId), stdin);
        categoryId[strcspn(categoryId, "\n")] = '\0';
        for (int i = 0; i < 10; i++) {
            if (strcmp(category[i].CategoryId, categoryId) == 0) {
                foundCategory = 1;
                break;
            }
        }
        if (!foundCategory) {
            printf("\tLoi: Ma danh muc khong ton tai!\n");
            return;
        }
        int foundProduct = 0;
        printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        printf("\t| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
        printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        for (int i = 0; i < numberProduct; i++) {
            if (strcmp(products[i].cateGory, categoryId) == 0) {
                foundProduct = 1;
                printf("\t| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n",
                       i + 1,
                       products[i].productId,
                       products[i].productName,
                       products[i].cateGory,
                       products[i].quantity,
                       products[i].price);
            }
        }
        if (!foundProduct) {
            printf("\tKhong co san pham nao!\n");
        } else {
            printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        }
    } else if (choice == 2) {
        // Loc theo khoang gia
        int startPrice, endPrice;
        printf("\tNhap gia bat dau: ");
        scanf("%d", &startPrice);
        getchar();
        printf("\tNhap gia ket thuc: ");
        scanf("%d", &endPrice);
        getchar();
        if (startPrice > endPrice || startPrice < 0 || endPrice < 0) {
            printf("\tLoi: Khoang gia khong hop le!\n");
            return;
        }
        int foundProduct = 0;
        printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        printf("\t| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
        printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        for (int i = 0; i < numberProduct; i++) {
            if (products[i].price >= startPrice && products[i].price <= endPrice) {
                foundProduct = 1;
                printf("\t| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n",
                       i + 1,
                       products[i].productId,
                       products[i].productName,
                       products[i].cateGory,
                       products[i].quantity,
                       products[i].price);
            }
        }
        if (!foundProduct) {
            printf("\tKhong co san pham nao!\n");
        } else {
            printf("\t+-----+----------+--------------------+------------+----------+----------+\n");
        }

    } else {
        printf("\tLua chon khong hop le!\n");
    }
}
void menuProduct() {
    int choice;
    do {
        printf("\t    PRODUCT MANAGEMENT MENU\n");
        printf("\t===========================\n");
        printf("\t[1]. Them san pham moi\n");
        printf("\t[2]. Hien thi danh sach san pham\n");
        printf("\t[3]. Chinh sua san pham\n");
        printf("\t[4]. Xoa san pham\n");
        printf("\t[5]. Tim kiem san pham\n");
        printf("\t[6]. Sap xep san pham\n");
        printf("\t[7]. Loc san pham\n");
        printf("\t[0]. Quay lai menu chinh\n");
        printf("\t===========================\n");
        printf("\tNhap lua chon: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                printf("\tQuay lai menu chinh.\n");
                break;
            case 1:
                addProduct();
                break;
            case 2:
                printProduct();
                break;
            case 3: 
                editProduct();  
                break;
			case 4: 
			    deleteProduct();
                break;
            case 5: 
                searchProductByName();
                break;
            case 6: 
                sortProductsByPrice();
                break;
            case 7:
                filterProducts();
                break;
            default:
                printf("\tLua chon khong hop le!\n");
        }
    } while (choice != 0);
}
void menuAdmin(){
	int choice;
	do{
		printf("\t	ADMIN MENU\n");
		printf("\t===========================\n");
		printf("\t[1]. Quan ly danh muc\n");
		printf("\t[2]. Quan ly san pham\n");  
		printf("\t[0]. Exit \n");
		printf("\t===========================\n");
		printf("\tEnter the choice: ");
		scanf("%d", &choice);	
		switch (choice){
			case 0:
				printf("Quay lai menu chinh\n");
				break;
			case 1:
				menuCategory();			
				break;
			case 2:
				menuProduct();
				break;
			default:
				printf("Lua chon khong hop le!\n");
		}
	}while (choice != 0);
}
void customerMenu() {
    int customerChoice;
    do {
        printf("\nMoi ban Mua Hang:\n");
        printf("1. Hien thi danh sach san pham\n");
        printf("2. Them san pham vao gio hang\n");
        printf("3. Xem gio hang\n");
        printf("4. Thanh toan\n");
        printf("5. Quay lai\n");
        printf("Nhap lua chon cua ban: ");
        if (scanf("%d", &customerChoice) != 1) {
            printf("Dau vao khong hop le. Vui long nhap so.\n");
            clearInputBuffer();
            continue;
        }
        switch (customerChoice) {
            case 1:
                displayProductList();
                break;
            case 2:
                addToCart();
                break;
            case 3:
                viewCart();
                break;
            case 4:
                checkout();
                break;
            case 5:
                printf("Quay lai menu chinh.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long nhap lai.\n");
                break;
        }
    } while (customerChoice != 5);
}
struct CartItem {
    struct Product product;
    int quantity;
};
struct CartItem cart[100];
int cartItemCount = 0;
//ham hien thi danh sach san pham 
void displayProductList() {
    if (numberProduct == 0) {
        printf("\nDanh sach san pham rong!\n");
        return;
    }
    printf("\nDanh sach san pham:\n");
    printf("+-----+----------+--------------------+------------+----------+----------+\n");
    printf("| STT | Ma SP    | Ten San Pham       | Danh Muc   | So Luong | Gia      |\n");
    printf("+-----+----------+--------------------+------------+----------+----------+\n");
    for (int i = 0; i < numberProduct; i++) {
        printf("| %-3d | %-8s | %-18s | %-10s | %-8d | %-8d |\n", 
               i + 1, 
               products[i].productId, 
               products[i].productName, 
               products[i].cateGory, 
               products[i].quantity, 
               products[i].price);
    }
    printf("+-----+----------+--------------------+------------+----------+----------+\n");
}
// ham them san pham vao cua hang 
void addToCart() {
    char productId[10];
    printf("\nNhap Ma SP san pham ban muon them vao gio hang: ");
    scanf("%s", productId);
    // Tim san pham trong danh sach
    int found = 0;
    for (int i = 0; i < numberProduct; i++) {
        if (strcmp(products[i].productId, productId) == 0) {
            found = 1;
            // Kiem tra san pham co trong gio hang chua
            int inCart = 0;
            for (int j = 0; j < cartItemCount; j++) {
                if (strcmp(cart[j].product.productId, productId) == 0) {
                    cart[j].quantity++;
                    printf("Da tang so luong san pham '%s' trong gio hang.\n", products[i].productName);
                    inCart = 1;
                    break;
                }
            }
            if (!inCart) {
                cart[cartItemCount].product = products[i];
                cart[cartItemCount].quantity = 1;
                cartItemCount++;
                printf("San pham '%s' da duoc them vao gio hang.\n", products[i].productName);
            }
            break;
        }
    }
    if (!found) {
        printf("San pham voi Ma SP '%s' khong ton tai.\n", productId);
    }
}	
// Ham xem gio hang 
void viewCart() {
    if (cartItemCount == 0) {
        printf("\nGio hang cua ban rong!\n");
        return;
    }
    printf("\nGio hang cua ban:\n");
    printf("+-----+--------------------+----------+----------+\n");
    printf("| STT | Ten San Pham       | So Luong | Gia      |\n");
    printf("+-----+--------------------+----------+----------+\n");
    int total = 0;
    for (int i = 0; i < cartItemCount; i++) {
        printf("| %-3d | %-18s | %-8d | %-8d |\n", 
               i + 1, 
               cart[i].product.productName, 
               cart[i].quantity, 
               cart[i].product.price * cart[i].quantity);
        total += cart[i].product.price * cart[i].quantity;
    }
    printf("+-----+--------------------+----------+----------+\n");
    printf("Tong cong: %d VND\n", total);
}
// ham thanh toan 
void checkout() {
    if (cartItemCount == 0) {
        printf("\nGio hang cua ban rong!\n");
        return;
    }
    printf("\nThanh toan don hang:\n");
    int total = 0;
    for (int i = 0; i < cartItemCount; i++) {
        total += cart[i].product.price * cart[i].quantity;
    }
    printf("Tong so tien: %d VND\n", total);
    printf("Don hang cua ban da duoc thanh toan thanh cong!\n");
    // Xoa gio hang sau khi thanh toan
    cartItemCount = 0;
}
void clearInputBuffer() {
    while (getchar() != '\n');
}
int main() {
	for (int i = 0; i < 10; i++) {
        category[i].CategoryId[0] = '\0';
        category[i].CategoryName[0] = '\0';
    }
    createAdminFileIfNotExists();
    loadCategoryFromFile();
    loadProductFromFile();
    int choice;
    do {
        printf("*** Store Management System Using C ***\n");
        printf("\n");
        printf("\tCHOOSE YOUR ROLE\n");
        printf("\t====================\n");
        printf("\t[1]. Admin\n");
        printf("\t[2]. Khach hang\n");
        printf("\t[0]. Thoat\n");
        printf("\t====================\n");
        printf("\tEnter your choice: ");
        if (scanf("%d", &choice) != 1) { 
            printf("\tDau vao khong hop le vui long nhap so.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 0:
                printf("\tBan da thoat khoi chuong trinh. \n");
                break;
            case 1:
                if (loginAccount()) {
                    menuAdmin();
                } else {
                    printf("\tDang nhap khong thanh cong vui long thu lai.\n");
                }
                break;
            case 2:
                customerMenu();
                break;
        }
    } while (choice != 0);
    saveCategoryToFile();
    saveProductToFile();
    return 0;
}

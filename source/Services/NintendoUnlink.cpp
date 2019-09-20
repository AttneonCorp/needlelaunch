// For future use in deltalaunch
// Copyright XorToll Goldleaf with some changes by me
#include "NintendoUnlink.hpp"
using namespace acc;
App app;
bool IsLinked()
    {
        u128 selected_user = SelectUser();
	BaasAdministrator admin;
        auto res = GetBaasAccountAdministrator(selected_user, &admin);
        if(res == 0)
        {
            bool linked = false;
            admin.IsLinkedWithNintendoAccount(&linked);
            admin.Close();
            return linked;
        }
        return false;
    }
u128 SelectUser()
    {
        u128 user = App::LaunchPSelect();
        if(user != 0){
	return user;
	}
	u128 urc = -1;
	Result rc = -1;
	app.ShowError("Failed to get username for this operation.\n","Here is the return code\n", rc);
	return urc;
    }
Result UnlinkLocally()
    {
	u128 selected_user = SelectUser();
        BaasAdministrator admin;
	Result error_code = -1;
        auto res = GetBaasAccountAdministrator(selected_user, &admin);
        if(res == 0)
        {
            bool linked = false;
            admin.IsLinkedWithNintendoAccount(&linked);
            if(linked) res = admin.DeleteRegistrationInfoLocally();
            admin.Close();
        }
	else {
	std::string error = "Failed to get username to unlink";
	app.ShowError(error,"\nThe error is", error_code);
	}
        return res;
}

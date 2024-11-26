import { NgModule } from "@angular/core";
import { AuthComponent } from "./components/auth/auth.component";
import { LoginComponent } from "./components/login/login.component";
import { RegistrationComponent } from "./components/registration/registration.component";
import { RouterModule, Routes } from "@angular/router";

const routes: Routes = [
    {
      path: '',
      component: AuthComponent,
      children: [
        {
          path: 'login',
          component: LoginComponent,
          data: { returnUrl: window.location.pathname },
        },
        {
          path: 'registration',
          component: RegistrationComponent,
        },
        { path: '', redirectTo: 'login', pathMatch: 'full' },
        { path: '**', redirectTo: 'login', pathMatch: 'full' },
      ],
    },
  ];
  
  @NgModule({
    imports: [RouterModule.forChild(routes)],
    exports: [RouterModule],
  })
  export class AuthRoutingModule {}
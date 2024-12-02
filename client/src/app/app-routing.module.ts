import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AuthGuard } from './modules/auth/services/auth.guard';

export const routes: Routes = [
    {
        path: 'auth',
        loadChildren: () => 
            import('./modules/auth/auth.module').then((m) => m.AuthModule)
    },
    /*
    {
        path: 'error',
        loadChildren: () =>
            import('./modules/errors/errors.module').then((m) => m.ErrorsModule),
    },
    */
    {
        path: 'dashboard',
        canActivate: [ AuthGuard ],
        loadChildren: () =>
           // import('./modules/layout/layout.module').then((m) => m.LayoutModule),
            import('./modules/pages/dashboard/dashboard.module').then((m) => m.DashboardModule)
      },
      { path: '', redirectTo: '/dashboard', pathMatch: 'full' },
      { path: '**', redirectTo: '/dashboard' },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

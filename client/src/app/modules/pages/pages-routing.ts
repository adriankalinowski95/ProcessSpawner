import { Routes } from '@angular/router';

const PagesRouting: Routes = [
    {
        path: 'dashboard',
        loadChildren: () => import('./dashboard/dashboard.module').then((m) => m.DashboardModule),
    },
    {
        path: '',
        redirectTo: '/dashboard',
        pathMatch: 'full',
    },
    {
      path: '**',
      redirectTo: 'error/404',
    },
  ];
  
  export { PagesRouting };
  
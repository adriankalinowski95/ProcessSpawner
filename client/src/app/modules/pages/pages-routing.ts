import { Routes } from '@angular/router';

const PagesRouting: Routes = [
    {
        path: 'dashboard',
        loadChildren: () => import('./dashboard/dashboard.module').then((m) => m.DashboardModule),
    },
    {
        path: 'process-spawner',
        loadChildren: () => import('./process-spawner/process-spawner.module').then((m) => m.ProcessSpawnerModule),
    },
    {
        path: 'process-manager',
        loadChildren: () => import('./process-manager/process-manager.module').then((m) => m.ProcessManagerModule),
    },
    {
        path: 'process-event',
        loadChildren: () => import('./process-event/process-event.module').then((m) => m.ProcessEventModule),
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
  
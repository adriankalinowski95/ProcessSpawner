import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { ProcessListComponent } from './components/process-list/process-list.component';

const routes: Routes = [
    {
        path: 'user/:id',
        component: ProcessListComponent,
        children: [
          { path: '**', redirectTo: '', pathMatch: 'full' },
        ],
    },
    {
        path: 'manager/:id',
        component: ProcessListComponent,
        children: [
          { path: '**', redirectTo: '', pathMatch: 'full' },
        ],
    },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class ProcessSpawnerRoutingModule { }

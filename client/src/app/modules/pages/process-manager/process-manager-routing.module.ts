import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { ProcessManagerListComponent } from './components/process-manager-list/process-manager-list.component';

const routes: Routes = [
    {
        path: '',
        component: ProcessManagerListComponent,
        children: [
          { path: '**', redirectTo: '', pathMatch: 'full' },
        ],
    },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class ProcessManagerRoutingModule { }

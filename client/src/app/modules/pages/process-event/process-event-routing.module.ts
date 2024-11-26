import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { ProcessEventListComponent } from './components/process-event-list/process-event-list.component';

const routes: Routes = [
    {
        path: 'process/:id',
        component: ProcessEventListComponent,
        children: [
          { path: '**', redirectTo: '', pathMatch: 'full' },
        ],
    }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class ProcessEventRoutingModule { }

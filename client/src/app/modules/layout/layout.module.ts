import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule, Routes } from '@angular/router';
import { LayoutComponent } from './components/layout/layout.component';
import { PagesRouting } from '../pages/pages-routing';

const routes: Routes = [
    {
      path: '',
      component: LayoutComponent,
      children: PagesRouting,
    },
];

@NgModule({
  declarations: [
    LayoutComponent
  ],
  imports: [
    CommonModule,
    RouterModule.forChild(routes),
  ],
  exports: [RouterModule]
})
export class LayoutModule { }

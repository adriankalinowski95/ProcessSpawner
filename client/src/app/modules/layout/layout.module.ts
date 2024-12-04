import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule, Routes } from '@angular/router';
import { LayoutComponent } from './components/layout/layout.component';
import { PagesRouting } from '../pages/pages-routing';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatMenuModule } from '@angular/material/menu';
import { ContentComponent } from './components/content/content.component';
import { SidenavContainerComponent } from './components/sidenav/components/sidenav-container/sidenav-container.component';
import { MatListModule } from '@angular/material/list';
import { ToolbarComponent } from './components/toolbar/toolbar.component';
import { MatTooltipModule } from '@angular/material/tooltip';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';
import { SidenavComponent } from './components/sidenav/components/sidenav/sidenav.component';

const routes: Routes = [
    {
      path: '',
      component: LayoutComponent,
      children: PagesRouting,
    },
];

@NgModule({
  declarations: [
    LayoutComponent,
    ContentComponent,
    SidenavContainerComponent,
    ToolbarComponent,
    SidenavComponent
  ],
  imports: [
    CommonModule,
    RouterModule.forChild(routes),
    MatToolbarModule,
    MatButtonModule,
    MatIconModule,
    MatSidenavModule,
    MatMenuModule,
    MatSidenavModule,
    MatListModule,
    MatTooltipModule,
    MatProgressSpinnerModule,
    MatExpansionModule,
  ],
  exports: [ RouterModule ]
})
export class LayoutModule { }

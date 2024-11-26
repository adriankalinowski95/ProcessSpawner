import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProcessManagerListComponent } from './process-manager-list.component';

describe('ProcessManagerListComponent', () => {
  let component: ProcessManagerListComponent;
  let fixture: ComponentFixture<ProcessManagerListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ProcessManagerListComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ProcessManagerListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
